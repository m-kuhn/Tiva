// $Id$


/**
* Implementation of Drupal behavior.
*/

(function ($) {

Drupal.behaviors.timelineAnimation = {
  attach: function (context) {
    var data = $(context).data('date_popup');
    for (var id in Drupal.settings.datePopup) {
      $('#'+ id).bind('focus', Drupal.settings.datePopup[id], function(e) {
      }
    }
  }
};

})(jQuery);