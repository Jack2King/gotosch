<!-- Start footer -->
    <footer id="mu-footer">
      <!-- start footer top -->
      <div class="mu-footer-top">
        <div class="container">
          <div class="mu-footer-top-area">
            <div class="row">
              <div class="lg-3 md-3 sm-3">
                <div class="mu-footer-widget">
                  <?php print render($page['footer_firstcolumn']); ?>
                </div>
              </div>
             
              <div class="lg-3 md-3 sm-3">
                <div class="mu-footer-widget">
                  <?php print render($page['footer_secondcolumn']); ?>
                </div>
              </div>
              <div class="lg-3 md-3 sm-3">
                <div class="mu-footer-widget">
                   <?php print render($page['footer_thirdcolumn']); ?>
                </div>
              </div>
              <div class="lg-3 md-3 sm-3">
                <div class="mu-footer-widget">
                  <?php print render($page['footer_fourthcolumn']); ?>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <!-- end footer top -->

      <!-- start footer bottom -->
      <div class="mu-footer-bottom">
        <div class="container">
          <div class="mu-footer-bottom-area">
            <?php print render($page['footer']); ?>
          </div>
        </div>
      </div>
      <!-- end footer bottom -->
    </footer>
    <!-- End footer -->

