/**
 * @license
 * Visual Blocks Language
 *
 * Copyright 2012 Google Inc.
 * https://developers.google.com/blockly/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @fileoverview Generating JavaScript for action blocks.
 * @author info@technikh.com (TechNikh)
 */
'use strict';

goog.provide('Blockly.JavaScript.actions');

goog.require('Blockly.JavaScript');


Blockly.JavaScript['actions_fill_textbox'] = function(block) {
  var text_locator = block.getFieldValue('LOCATOR');
  var text_text = block.getFieldValue('TEXT');
  var code = '$driver->findElement(WebDriverBy::cssSelector('+text_locator+'))->sendKeys('+text_text+');\n';
  return code;
};

