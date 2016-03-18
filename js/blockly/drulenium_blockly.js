/*
 * @file
 * JavaScript for blockly integration in preset create/edit form.
 *
 */

(function($) {

	Drupal.behaviors.druleniumBlockly = {
		attach : function() {
			var workspace = Blockly.inject('blocklyDiv',
			        {media: 'sites/all/libraries/blockly/media/',
			         toolbox: document.getElementById('toolbox')}
			);
			var data = $('input[name=blockly_workspace_data]').val();
			if (data) {
			  BlocklyStorage.loadXml_(data, workspace);
			}
		},
		save : function (){
	     	var workspace = Blockly.getMainWorkspace();
			var xml = Blockly.Xml.workspaceToDom(workspace);
			var data = Blockly.Xml.domToText(xml);
			$('input[name=blockly_workspace_data]').val(data);
			alert('Workspace saved!');
	     }
	};
	


})(jQuery);
