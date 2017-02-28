<?php

/**
 * @file
 * Drupal stream wrapper implementation for Aliyun OSS
 *
 * Implements DrupalStreamWrapperInterface to provide an Aliyun OSS wrapper with
 * the oss:// prefix
 */
class AliyunOSSStreamWrapper implements DrupalStreamWrapperInterface {

  /**
   * @var String Instance URI referenced as "oss://bucket/key"
   */
  protected $uri;

  /**
   * @var Aliyun OSS connection object
   */
  protected $oss = NULL;

  /**
   * @var Aliyun OSS access key id
   */
  protected $oss_key_id;

  /**
   * @var Aliyun OSS access key secret
   */
  protected $oss_key_secret;

  /**
   * @var string OSS bucket name
   */
  protected $bucket;
  protected $endpoint;
  /**
   * @var string Domain we use to access files over http
   */
  protected $domain = NULL;

  /**
   * @var int Current read/write position
   */
  protected $position = 0;

  /**
   * @var int Total size of the object as returned by OSS (Content-length)
   */
  protected $object_size = 0;

  /**
   * @var string Object read/write buffer, typically a file
   */
  protected $buffer = NULL;

  /**
   * @var boolean Whether $buffer is active as a write buffer
   */
  protected $buffer_write = FALSE;

  /**
   * @var int Buffer length
   */
  protected $buffer_length = 0;

  /**
   * @var array directory listing
   */
  protected $dir = array();

  /**
   * @var array Default map for determining file mime types
   */
  protected static $mapping = NULL;

  /**
   * @var boolean Whether local file metadata caching is on
   */
  protected $caching = FALSE;

  /**
   * @var array Map for files that should have their Content-disposition header
   * set to force "save as".
   */
  protected $saveas = array();

  /**
   * @var array Map for files that should have a URL will be created that times
   * out in a designated number of seconds.
   */
  protected $sign_urls = array();

  /**
   * Object constructor
   *
   * Sets the OSS key_id, key_secret and the bucket name
   */
  public function __construct() {
    $this->oss_key_id = variable_get('aliyunoss_key_id', '');
    $this->oss_key_secret = variable_get('aliyunoss_key_secret', '');
    $this->bucket = $bucket = variable_get('aliyunoss_bucket', '');
    $this->endpoint = variable_get('aliyunoss_endpoint', 'oss.aliyuncs.com');
    // CNAME support for customising OSS URLs
    if (variable_get('aliyunoss_cname', 0)) {
      $domain = variable_get('aliyunoss_domain', '');
      if (strlen($domain) > 0) {
        $this->domain = 'http://' . $domain;
      }
      else {
        $this->domain = 'http://' . $this->bucket;
      }
    }
    else {
      $this->domain = 'http://' . $this->bucket . "." . variable_get('aliyunoss_endpoint', 'oss.aliyuncs.com');
    }

    // Check whether local file caching is turned on
    if (variable_get('aliyunoss_cache', FALSE)) {
      $this->caching = TRUE;
    }

    // Presigned url-list
    $sign_urls = explode("\n", variable_get('aliyunoss_sign_urls', ''));
    $sign_urls = array_map('trim', $sign_urls);
    $sign_urls = array_filter($sign_urls, 'strlen');
    $this->sign_urls = array();
    foreach ($sign_urls as $sign_url) {
      // Check for an explicit key.
      $matches = array();
      if (preg_match('/(.*)\|(.*)/', $sign_url, $matches)) {
        $this->sign_urls[$matches[2]] = $matches[1];
      }
      else {
        $this->sign_urls[$sign_url] = 60;
      }
    }

    // Force "save as" list
    $saveas = explode("\n", variable_get('aliyunoss_saveas', ''));
    $saveas = array_map('trim', $saveas);
    $saveas = array_filter($saveas, 'strlen');
    $this->saveas = $saveas;

  }

  /**
   * Sets the stream resource URI.
   *
   * URIs are formatted as "oss://bucket/key"
   *
   * @return
   *   Returns the current URI of the instance.
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Returns the stream resource URI.
   *
   * URIs are formatted as "oss://bucket/key"
   *
   * @return
   *   Returns the current URI of the instance.
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Returns a web accessible URL for the resource.
   *
   * In the format http://bucket.oss.aliyuncs.com/object.jpg
   *
   * @return
   *   Returns a string containing a web accessible URL for the resource.
   */
  public function getExternalUrl() {

    // Image styles support
    // Delivers the first request to an image from the private file system
    // otherwise it returns an external URL to an image that has not been
    // created yet
    $path = explode('/', $this->getLocalPath());
    if ($path[0] == 'styles') {
      if (!$this->_aliyunoss_get_object($this->uri, $this->caching)) {
        array_shift($path);
        return url('system/files/styles/' . implode('/', $path), array('absolute' => TRUE));
      }
    }

    $local_path = $this->getLocalPath();

    $info = array(
      'download_type' => 'http',
      'sign_url' => FALSE,
      'sign_url_timeout' => 60,
    );

    // Allow other modules to change the download link type.
    $info = array_merge($info, module_invoke_all('aliyunoss_url_info', $local_path, $info));

    // Signed URLs
    if (!$info['sign_url']) {
      foreach ($this->sign_urls as $path => $timeout) {
        if (preg_match('#' . strtr($path, '#', '\#') . '#', $local_path)) {
          $info['sign_url'] = TRUE;
          $info['sign_url_timeout'] = $timeout;
          break;
        }
      }
    }
    // Save as
    if ($info['download_type'] != 'torrent') {
      foreach ($this->saveas as $path) {
        if (preg_match('#' . strtr($path, '#', '\#') . '#', $local_path)) {
          $info['response']['content-disposition'] = 'attachment; filename=' . basename($local_path);
          break;
        }
      }
    }

    $timeout = ($info['sign_url']) ? time() + $info['sign_url_timeout'] : 0;

    if ($info['sign_url']) {
      $url = $this->getOSS()
        ->get_sign_url($this->bucket, $local_path, $timeout);
      return $url;
    }

    $url = $this->domain . '/' . $local_path;
    return $url;
  }

  /**
   * Determine a file's media type
   *
   * Uses Drupal's mimetype mappings. Returns 'application/octet-stream' if
   * no match is found.
   *
   * @return
   *   Returns a string representing the file's MIME type
   */
  public static function getMimeType($uri, $mapping = NULL) {

    // Load the default file map
    if (!isset(self::$mapping)) {
      include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
      self::$mapping = file_mimetype_mapping();
    }

    $extension = '';
    $file_parts = explode('.', basename($uri));

    // Remove the first part: a full filename should not match an extension.
    array_shift($file_parts);

    // Iterate over the file parts, trying to find a match.
    // For my.awesome.image.jpeg, we try:
    //   - jpeg
    //   - image.jpeg, and
    //   - awesome.image.jpeg
    while ($additional_part = array_pop($file_parts)) {
      $extension = strtolower($additional_part . ($extension ? '.' . $extension : ''));
      if (isset(self::$mapping['extensions'][$extension])) {
        return self::$mapping['mimetypes'][self::$mapping['extensions'][$extension]];
      }
    }

    return 'application/octet-stream';
  }

  /**
   * Changes permissions of the resource.
   *
   * OSS doesn't have anything to do, so just returns TRUE;
   *
   * @param $mode
   *   Integer value for the permissions. Consult PHP chmod() documentation
   *   for more information.
   *
   * @return
   *   Returns TRUE.
   */
  public function chmod($mode) {
    $this->assertConstructorCalled();
    return TRUE;
  }

  /**
   * Returns canonical, absolute path of the resource.
   *
   * @return
   *   Returns FALSE as this wrapper does not provide an implementation.
   */
  public function realpath() {
    return FALSE;
  }

  /**
   * Gets the name of the directory from a given path.
   *
   * This method is usually accessed through drupal_dirname(), which wraps
   * around the normal PHP dirname() function, which does not support stream
   * wrappers.
   *
   * @param $uri
   *   An optional URI.
   *
   * @return
   *   A string containing the directory name, or FALSE if not applicable.
   *
   * @see drupal_dirname()
   */
  public function dirname($uri = NULL) {
    list($scheme, $target) = explode('://', $uri, 2);
    $target = $this->getTarget($uri);
    $dirname = dirname($target);

    if ($dirname == '.') {
      $dirname = '';
    }

    return $scheme . '://' . $dirname;
  }

  /**
   * Support for fopen(), file_get_contents(), file_put_contents() etc.
   *
   * @param $uri
   *   A string containing the URI to the file to open.
   * @param $mode
   *   The file mode ("r", "wb" etc.).
   * @param $options
   *   A bit mask of STREAM_USE_PATH and STREAM_REPORT_ERRORS.
   * @param $opened_path
   *   A string containing the path actually opened.
   *
   * @return
   *   Returns TRUE if file was opened successfully.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-open.php
   */
  public function stream_open($uri, $mode, $options, &$opened_path) {
    $this->uri = $uri;
    // if this stream is being opened for writing, clear the object buffer
    // Return true as we'll create the object on flush call
    if (strpbrk($mode, 'wax')) {
      $this->clearBuffer();
      $this->write_buffer = TRUE;
      return TRUE;
    }
    $metadata = $this->_aliyunoss_get_object($uri, $this->caching);
    if ($metadata) {
      $this->clearBuffer();
      $this->write_buffer = FALSE;
      $this->object_size = $metadata['filesize'];
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Support for fclose().
   *
   * Clears the object buffer and returns TRUE
   *
   * @return
   *   TRUE
   *
   * @see http://php.net/manual/en/streamwrapper.stream-close.php
   */
  public function stream_close() {
    $this->clearBuffer();
    return TRUE;
  }

  /**
   * Support for flock().
   *
   * @param $operation
   *   One of the following:
   *   - LOCK_SH to acquire a shared lock (reader).
   *   - LOCK_EX to acquire an exclusive lock (writer).
   *   - LOCK_UN to release a lock (shared or exclusive).
   *   - LOCK_NB if you don't want flock() to block while locking (not
   *     supported on Windows).
   *
   * @return
   *   returns TRUE if lock was successful
   *
   * @see http://php.net/manual/en/streamwrapper.stream-lock.php
   */
  public function stream_lock($operation) {
    return FALSE;
  }

  /**
   * Support for fread(), file_get_contents() etc.
   *
   * @param $count
   *   Maximum number of bytes to be read.
   *
   * @return
   *   The string that was read, or FALSE in case of an error.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-read.php
   */
  public function stream_read($count) {
    try {
      // make sure that count doesn't exceed object size
      if ($count + $this->position > $this->object_size) {
        $count = $this->object_size - $this->position;
      }
      $object = $this->getLocalPath();
      $data = '';
      if ($count > 0) {
        $range_end = $this->position + $count - 1;
        if ($range_end > $this->buffer_length) {
          $content = $this->getOSS()->getObject($this->bucket, $object);
          $this->buffer .= $content;
          $this->buffer_length += strlen($content);
        }
        $data = substr($this->buffer, $this->position, $count);
        $this->position += strlen($data);
      }
      return $data;
    } catch (\OSS\Core\OssException$e) {
      return NULL;
    }
  }

  /**
   * Support for fwrite(), file_put_contents() etc.
   *
   * @param $data
   *   The string to be written.
   *
   * @return
   *   The number of bytes written (integer).
   *
   * @see http://php.net/manual/en/streamwrapper.stream-write.php
   */
  public function stream_write($data) {
    $data_length = strlen($data);
    $this->buffer .= $data;
    $this->buffer_length += $data_length;
    $this->position += $data_length;

    return $data_length;
  }

  /**
   * Support for feof().
   *
   * @return
   *   TRUE if end-of-file has been reached.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-eof.php
   */
  public function stream_eof() {
    if (!$this->uri) {
      return TRUE;
    }
    return ($this->position >= $this->object_size);
  }

  /**
   * Support for fseek().
   *
   * @param $offset
   *   The byte offset to got to.
   * @param $whence
   *   SEEK_SET, SEEK_CUR, or SEEK_END.
   *
   * @return
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-seek.php
   */
  public function stream_seek($offset, $whence) {
    switch ($whence) {
      case SEEK_CUR:
        // Set position to current location plus $offset
        $new_position = $this->position + $offset;
        break;
      case SEEK_END:
        // Set position to eof plus $offset
        $new_position = $this->object_size + $offset;
        break;
      case SEEK_SET:
      default:
        // Set position equal to $offset
        $new_position = $offset;
        break;
    }

    $ret = ($new_position >= 0 && $new_position <= $this->object_size);
    if ($ret) {
      $this->position = $new_position;
    }
    return $ret;
  }

  /**
   * Support for fflush(). Flush current cached stream data to storage.
   *
   * @return
   *   TRUE if data was successfully stored (or there was no data to store).
   *
   * @see http://php.net/manual/en/streamwrapper.stream-flush.php
   */
  public function stream_flush() {
    try {
      $this->getOSS()
        ->putObject($this->bucket, $this->getLocalPath(), $this->buffer);
      $this->clearBuffer();
      return TRUE;
    } catch (\OSS\Core\OssException $e) {
      return FALSE;
    }
  }

  /**
   * Support for ftell().
   *
   * @return
   *   The current offset in bytes from the beginning of file.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-tell.php
   */
  public function stream_tell() {
    return $this->position;
  }

  /**
   * Support for fstat().
   *
   * @return
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-stat.php
   */
  public function stream_stat() {
    return $this->_stat();
  }

  /**
   * Support for unlink().
   *
   * @param $uri
   *   A string containing the uri to the resource to delete.
   *
   * @return
   *   TRUE if resource was successfully deleted.
   *
   * @see http://php.net/manual/en/streamwrapper.unlink.php
   */
  public function unlink($uri) {
    $this->assertConstructorCalled();
    try {
      $this->getOSS()
        ->deleteObject($this->bucket, $this->getLocalPath($uri));
      db_delete('aliyunoss_file')
        ->condition('uri', $uri)
        ->execute();
      return TRUE;
    } catch (\OSS\Core\OssException $e) {
      return FALSE;
    }
  }

  /**
   * Support for rename().
   *
   * If $to_uri exists, this file will be overwritten. This behavior is
   * identical to the PHP rename() function.
   *
   * @param $from_uri
   *   The uri to the file to rename.
   * @param $to_uri
   *   The new uri for file.
   *
   * @return
   *   TRUE if file was successfully renamed.
   *
   * @see http://php.net/manual/en/streamwrapper.rename.php
   */
  public function rename($from_uri, $to_uri) {
    try {
      $this->getOSS()
        ->copyObject($this->bucket, $this->getLocalPath($from_uri), $this->bucket, $this->getLocalPath($to_uri));
      $this->unlink($from_uri);
      return TRUE;
    } catch (\OSS\Core\OssException $e) {
      return FALSE;
    }
  }

  /**
   * Returns the local writable target of the resource within the stream.
   *
   * This function should be used in place of calls to realpath() or similar
   * functions when attempting to determine the location of a file. While
   * functions like realpath() may return the location of a read-only file, this
   * method may return a URI or path suitable for writing that is completely
   * separate from the URI used for reading.
   *
   * @param $uri
   *   Optional URI.
   *
   * @return
   *   Returns a string representing a location suitable for writing of a file,
   *   or FALSE if unable to write to the file such as with read-only streams.
   */
  protected function getTarget($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    list($scheme, $target) = explode('://', $uri, 2);

    // Remove erroneous leading or trailing forward-slashes and backslashes.
    return trim($target, '\/');
  }

  /**
   * Support for mkdir().
   *
   * @param $uri
   *   A string containing the URI to the directory to create.
   * @param $mode
   *   Permission flags - see mkdir().
   * @param $options
   *   A bit mask of STREAM_REPORT_ERRORS and STREAM_MKDIR_RECURSIVE.
   *
   * @return
   *   TRUE if directory was successfully created.
   *
   * @see http://php.net/manual/en/streamwrapper.mkdir.php
   */
  public function mkdir($uri, $mode, $options) {
    $this->assertConstructorCalled();
    $localpath = $this->getLocalPath($uri);
    try {
      $this->getOSS()->createObjectDir($this->bucket, $localpath);
      return TRUE;
    } catch (\OSS\Core\OssException$e) {
      return FALSE;
    }
  }

  /**
   * Support for rmdir().
   *
   * @param $uri
   *   A string containing the URI to the directory to delete.
   * @param $options
   *   A bit mask of STREAM_REPORT_ERRORS.
   *
   * @return
   *   TRUE if directory was successfully removed.
   *
   * @see http://php.net/manual/en/streamwrapper.rmdir.php
   */
  public function rmdir($uri, $options) {
    $this->assertConstructorCalled();
    try {
      if ($this->_aliyunoss_is_dir($uri)) {
        $object = $this->getLocalPath($uri);
        $this->getOSS()->deleteObject($this->bucket, $object);
        return TRUE;
      }
      return FALSE;
    } catch (\OSS\Core\OssException $e) {
      return FALSE;
    }
  }

  /**
   * Support for stat().
   *
   * @param $uri
   *   A string containing the URI to get information about.
   * @param $flags
   *   A bit mask of STREAM_URL_STAT_LINK and STREAM_URL_STAT_QUIET.
   *
   * @return
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/en/streamwrapper.url-stat.php
   */
  public function url_stat($uri, $flags) {
    $this->assertConstructorCalled();
    return $this->_stat($uri);
  }

  /**
   * Support for opendir().
   *
   * @param $uri
   *   A string containing the URI to the directory to open.
   * @param $options
   *   Unknown (parameter is not documented in PHP Manual).
   *
   * @return
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-opendir.php
   */
  public function dir_opendir($uri, $options) {
    $this->assertConstructorCalled();
    if ($uri == NULL) {
      return FALSE;
    }
    else {
      if (!$this->_aliyunoss_is_dir($uri)) {
        return FALSE;
      }
    }

    $this->dir = array();

    $path = $this->getLocalPath($uri);

    $truncated = TRUE;

    $nextMarker = '';
    if (strlen($path) == 0) {
      $prefix = $path;
    }
    else {
      $prefix = $path . '/';
    }
    $prefix_length = strlen($prefix);
    $delimiter = '/';

    while ($truncated) {
      try {

        $options = array(
          'delimiter' => $delimiter,
          'prefix' => $prefix,
          'max-keys' => 1000,
          'marker' => $nextMarker,
        );

        $listObjectInfo = $this->getOSS()->listObjects($this->bucket, $options);
        $this->dir[] = '.';
        $this->dir[] = '..';
        $objectList = $listObjectInfo->getObjectList(); // 文件列表
        $prefixList = $listObjectInfo->getPrefixList(); // 目录列表
        $this->dir[] = $listObjectInfo->getPrefix();
        $nextMarker = $listObjectInfo->getNextMarker();

        foreach ($objectList as $obj) {
          if ($obj instanceof \OSS\Model\ObjectInfo) {
            $key = $obj->getKey();
            if (substr_compare($key, '/', -1, 1) !== 0) {
              $this->dir[] = $key;
            }
          }
        }
        foreach ($prefixList as $obj) {
          if ($obj instanceof \OSS\Model\PrefixInfo) {
            $this->dir[] = $obj->getPrefix();
          }
        }
        $getIsTruncated = $listObjectInfo->getIsTruncated();
        if (!isset($getIsTruncated) || $getIsTruncated == 'false') {
          $truncated = FALSE;
        }
        return TRUE;
      } catch (\OSS\Core\OssException $e) {
        return FALSE;
      }
    }
    return TRUE;
  }


  /**
   * Support for readdir().
   *
   * @return
   *   The next filename, or FALSE if there are no more files in the directory.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-readdir.php
   */
  public function dir_readdir() {
    $filename = current($this->dir);
    if ($filename !== FALSE) {
      next($this->dir);
    }
    return $filename;
  }

  /**
   * Support for rewinddir().
   *
   * @return
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-rewinddir.php
   */
  public function dir_rewinddir() {
    reset($this->dir);
    return TRUE;
  }

  /**
   * Support for closedir().
   *
   * @return
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-closedir.php
   */
  public function dir_closedir() {
    $this->dir = array();
    return TRUE;
  }

  /**
   * Return the local filesystem path.
   *
   * @param $uri
   *   Optional URI, supplied when doing a move or rename.
   */
  protected function getLocalPath($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    $path = str_replace('oss://', '', $uri);
    $path = trim($path, '/');
    return $path;
  }

  /**
   * Gets the path that the wrapper is responsible for.
   *
   * @return
   *   String specifying the path.
   */
  public function getDirectoryPath() {
    return $this->domain;
  }

  /**
   * Flush the object buffers
   */
  protected function clearBuffer() {
    $this->position = 0;
    $this->object_size = 0;
    $this->buffer = NULL;
    $this->buffer_write = FALSE;
    $this->buffer_length = 0;
  }

  /**
   * Get the OSS connection object
   * @return
   *   OSS connection object (AliyunOSS)
   */
  protected function getOSS() {
    if ($this->oss == NULL) {
      $bucket = variable_get('aliyunoss_bucket', '');
      $this->oss = new OSS\OssClient($this->oss_key_id, $this->oss_key_secret, $this->endpoint);
      $this->bucket = $bucket;
    }
    return $this->oss;
  }

  /**
   * Get file status
   *
   * @return
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-stat.php
   */
  protected function _stat($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    $metadata = $this->_aliyunoss_get_object($uri, $this->caching);
    if ($metadata) {
      $stat = array();
      $stat[0] = $stat['dev'] = 0;
      $stat[1] = $stat['ino'] = 0;
      $stat[2] = $stat['mode'] = $metadata['mode'];
      $stat[3] = $stat['nlink'] = 0;
      $stat[4] = $stat['uid'] = 0;
      $stat[5] = $stat['gid'] = 0;
      $stat[6] = $stat['rdev'] = 0;
      $stat[7] = $stat['size'] = 0;
      $stat[8] = $stat['atime'] = 0;
      $stat[9] = $stat['mtime'] = 0;
      $stat[10] = $stat['ctime'] = 0;
      $stat[11] = $stat['blksize'] = 0;
      $stat[12] = $stat['blocks'] = 0;

      if (!$metadata['dir']) {
//        $stat[4] = $stat['uid'] = $metadata['uid'];
        $stat[7] = $stat['size'] = $metadata['filesize'];
        $stat[8] = $stat['atime'] = $metadata['timestamp'];
        $stat[9] = $stat['mtime'] = $metadata['timestamp'];
        $stat[10] = $stat['ctime'] = $metadata['timestamp'];
      }
      return $stat;
    }
    return FALSE;
  }


  /**
   * Determine whether the $uri is a directory
   *
   * @param $uri
   *   A string containing the uri to the resource to check. If none is given
   *   defaults to $this->uri
   *
   * @return
   *   TRUE if the resource is a directory
   */
  protected function _aliyunoss_is_dir($uri = NULL) {
    if ($uri == NULL) {
      $uri = $this->uri;
    }
    try {
      $path = $this->getLocalPath($uri);
      if (strlen($path) === 0) {
        return TRUE;
      }
      $maxkeys = 1;
      $options = array(
        'prefix' => $path,
        'max-keys' => $maxkeys,
      );
      $listObjectInfo = $this->getOSS()->listObjects($this->bucket, $options);
      if (!empty($listObjectInfo->getPrefixList())) {
        return TRUE;
      }
      return FALSE;
    } catch (\OSS\Core\OssException $e) {
      return FALSE;
    }
  }

  /**
   * CACHING FUNCTIONS
   */

  /**
   * Try to fetch an object from the metadata cache, otherwise fetch it's
   * info from OSS and populate the cache.
   *
   * @param uri
   *   A string containing the uri of the resource to check.
   * @param $cach
   *   A boolean representing whether to check the cache for file information.
   *
   * @return
   *    An array if the $uri exists, otherwise FALSE.
   */
  protected function _aliyunoss_get_object($uri, $cache = TRUE) {
    if ($uri == 'oss://' || $uri == 'oss:') {
      $metadata = $this->_aliyunoss_format_response('/', array(), TRUE);
      return $metadata;
    }
    else {
      $uri = rtrim($uri, '/');
    }

    if ($cache) {
      $metadata = $this->_aliyunoss_get_cache($uri);
      if ($metadata) {
        return $metadata;
      }
    }

    $is_dir = $this->_aliyunoss_is_dir($uri);
    $metadata = NULL;
    if ($is_dir) {
      $metadata = $this->_aliyunoss_format_response($uri, array(), TRUE);
    }
    else {
      try {
        $response = $this->getOSS()
          ->getObjectMeta($this->bucket, $this->getLocalPath($uri));
        $metadata = $this->_aliyunoss_format_response($uri, $response);
      } catch (\OSS\Core\OssException $e) {
        return FALSE;
      }
    }
    if (is_array($metadata)) {
      // Save to the cache
      db_merge('aliyunoss_file')
        ->key(array('uri' => $metadata['uri']))
        ->fields($metadata)
        ->execute();
      return $metadata;
    }
    return FALSE;
  }

  /**
   * Fetch an object from the local metadata cache
   *
   * @param uri
   *  A string containing the uri of the resource to check.
   *
   * @return
   *    An array if the $uri is in the cache, otherwise FALSE
   */
  protected function _aliyunoss_get_cache($uri) {
    // Check cache for existing object.
    $result = db_query("SELECT * FROM {aliyunoss_file} WHERE uri = :uri", array(
      ':uri' => $uri,
    ));
    $record = $result->fetchAssoc();
    if ($record) {
      return $record;
    }
    return FALSE;
  }

  /**
   * Format returned file information from OSS into an array
   *
   * @param $uri
   *   A string containing the uri of the resource to check.
   * @param $response
   *   An object containing the collective metadata for the Aliyun OSS object
   * @param $is_dir
   *   A boolean indicating whether this object is a directory.
   *
   * @return
   *   An array containing formatted metadata
   */
  protected function _aliyunoss_format_response($uri, $response, $is_dir = FALSE) {
    $metadata = array('uri' => $uri);
    if (isset($response['content-length'])) {
      $metadata['filesize'] = $response['content-length'];
    }

    //[last-modified] => Thu, 14 Apr 2016 09:55:00 GMT
    if (isset($response['last-modified'])) {
      $metadata['timestamp'] = date('U', strtotime((string) $response['last-modified']));
    }

    if ($is_dir) {
      $metadata['dir'] = 1;
      $metadata['mode'] = 0040000; // S_IFDIR indicating directory
      $metadata['mode'] |= 0777;
    }
    else {
      $metadata['dir'] = 0;
      $metadata['mode'] = 0100000; // S_IFREG indicating file
      $metadata['mode'] |= 0777; // everything is writeable
    }
    return $metadata;
  }

  /**
   * Assert that the constructor has been called, call it if not.
   *
   * Due to PHP bug #40459, the constructor of this class isn't always called
   * for some of the methods. This private method calls the constructor if
   * it hasn't been called before.
   *
   * @see https://bugs.php.net/bug.php?id=40459
   */
  private function assertConstructorCalled() {
    if ($this->domain === NULL) {
      $this->__construct();
    }
  }
}
