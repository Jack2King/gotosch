<!--START SCROLL TOP BUTTON -->
    <a class="scrollToTop" href="#">
      <i class="fa fa-angle-up"></i>
    </a>
    <!-- END SCROLL TOP BUTTON -->

<!-- Start header -->
<header>
   <div id="topbar">
     <div class="container">
        <div class="row">
         <div class="info contact">
            <?php print render($page['contact']); ?>
         </div>
         <div class="info login">
           <p><?php if ($logged_in): ?>
             <strong>Welcome <a href="<?php print url('user'); ?>"><?php print t("@user", array('@user' => $logged_in ? $user->name : t("guest"))); ?></a></strong> | <strong><?php print l(t("Sign out"), 'user/logout'); ?><strong>
             <?php else: ?>
             <strong><a href="<?php print url('user/login'); ?>"><?php print t("Sign in"); ?></a></strong> | <strong><a href="<?php print url('user/register'); ?>"><?php print t("Sign up"); ?></a><?php endif; ?></strong>
           </p>
         </div>
      </div>
     </div>
   </div>
   <nav>
     <div class="container">
        <div class="logo">
            <a href="<?php print $front_page; ?>"><img src="<?php print $base_path . $directory; ?>/img/logo.jpg" alt="Gotosch"></a>
        </div>
        <div class="menu">
          <?php print render($page['menu']); ?>
        </div>
      </div>
   </nav>
</header>

  <!-- Start Slider -->
  <section id="mu-slider">
   <?php print render($page['slideshow']); ?>
  </section>
  <!-- End Slider -->

  <!-- Start service  -->
  <section id="mu-service">
    <?php print render($page['service']); ?>
  </section>
  <!-- End service  -->

  <!-- Start about us -->
  <section id="mu-about-us">
    <?php print render($page['about']); ?>
  </section>
  <!-- End about us -->

  <!-- Start about us counter -->
   <section id="mu-abtus-counter">
    <?php print render($page['counter']); ?>
   </section>
  <!-- End about us counter -->

  <!-- Start features section -->
  <section id="mu-features">
    <?php print render($page['features']); ?>
  </section>
  <!-- End features section -->
 

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
