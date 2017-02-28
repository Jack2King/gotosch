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
             <strong><a href="<?php print url('user'); ?>"><?php print t("welcome @user", array('@user' => $logged_in ? $user->name : t("guest"))); ?></a></strong> | <strong><?php print l(t("Sign out"), 'user/logout'); ?><strong>
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
            <a href="<?php print $front_page; ?>"><img src="<?php print $base_path . $directory; ?>/assets/img/logo.jpg" alt="Gotosch"></a>
        </div>
        <div class="menu">
          <?php print render($page['menu']); ?>
        </div>
      </div>
   </nav>
</header>
