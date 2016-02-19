/*
 * @file
 * JavaScript for weight table in preset create/edit form.
 *
 */

(function($) {

	// Re-enable form elements that are disabled for non-ajax situations.
	Drupal.behaviors.druleniumPresetForm = {
		attach : function() {
			// Debugging
			/*$("#plugin-items-table input[type=text]").change(function(){

			});*/
			// On save button submit
			$("#edit-submit").click(function () {
				//alert("submit clicked");
				// TODO: this submit clicked is being executed more than once like 3 times, when table elements are dragged for re-ordering
				$('#plugin-items-table input[type=text]').each(function() {
					var classes = $(this).attr('class');
					var classesSelector = classes.replace(/ /g, ".");
					$("."+classesSelector).not("#plugin-items-table input").val($(this).val());
					//alert($("."+classesSelector).not("#plugin-items-table input").val());
				});
				$('#plugin-items-table select.form-select').each(function() {
					var classes = $(this).attr('class');
					var classesSelector = classes.replace(/ /g, ".");
					$("."+classesSelector).not("#plugin-items-table select").val($(this).val());
					//alert($("."+classesSelector).not("#plugin-items-table input").val());
				});
				//Weights Sync
				$('#plugin-items-table .plugin-item-weight').each(function() {
					var classes = $(this).attr('class');
					var classesSelector = classes.replace(/ /g, ".");
					var weight = $(this).val();
					$("."+classesSelector).not("#plugin-items-table .plugin-item-weight").val(weight);
					//alert($("."+classesSelector).not("#plugin-items-table .plugin-item-weight").val());
				});
			});
		}
	};

})(jQuery);
