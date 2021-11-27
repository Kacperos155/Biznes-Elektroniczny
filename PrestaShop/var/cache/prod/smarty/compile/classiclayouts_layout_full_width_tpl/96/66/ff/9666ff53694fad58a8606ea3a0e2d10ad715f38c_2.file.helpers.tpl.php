<?php
/* Smarty version 3.1.39, created on 2021-11-27 01:59:45
  from 'C:\Users\Kacpe\source\repos\- Studia\Biznes Elektroniczny\PrestaShop\themes\classic\templates\_partials\helpers.tpl' */

/* @var Smarty_Internal_Template $_smarty_tpl */
if ($_smarty_tpl->_decodeProperties($_smarty_tpl, array (
  'version' => '3.1.39',
  'unifunc' => 'content_61a1830173f290_75255023',
  'has_nocache_code' => false,
  'file_dependency' => 
  array (
    '9666ff53694fad58a8606ea3a0e2d10ad715f38c' => 
    array (
      0 => 'C:\\Users\\Kacpe\\source\\repos\\- Studia\\Biznes Elektroniczny\\PrestaShop\\themes\\classic\\templates\\_partials\\helpers.tpl',
      1 => 1636104739,
      2 => 'file',
    ),
  ),
  'includes' => 
  array (
  ),
),false)) {
function content_61a1830173f290_75255023 (Smarty_Internal_Template $_smarty_tpl) {
$_smarty_tpl->smarty->ext->_tplFunction->registerTplFunctions($_smarty_tpl, array (
  'renderLogo' => 
  array (
    'compiled_filepath' => 'C:\\Users\\Kacpe\\source\\repos\\- Studia\\Biznes Elektroniczny\\PrestaShop\\var\\cache\\prod\\smarty\\compile\\classiclayouts_layout_full_width_tpl\\96\\66\\ff\\9666ff53694fad58a8606ea3a0e2d10ad715f38c_2.file.helpers.tpl.php',
    'uid' => '9666ff53694fad58a8606ea3a0e2d10ad715f38c',
    'call_name' => 'smarty_template_function_renderLogo_162588955761a183016fa658_75833604',
  ),
));
?> 

<?php }
/* smarty_template_function_renderLogo_162588955761a183016fa658_75833604 */
if (!function_exists('smarty_template_function_renderLogo_162588955761a183016fa658_75833604')) {
function smarty_template_function_renderLogo_162588955761a183016fa658_75833604(Smarty_Internal_Template $_smarty_tpl,$params) {
foreach ($params as $key => $value) {
$_smarty_tpl->tpl_vars[$key] = new Smarty_Variable($value, $_smarty_tpl->isRenderingCache);
}
?>

  <a href="<?php echo htmlspecialchars($_smarty_tpl->tpl_vars['urls']->value['pages']['index'], ENT_QUOTES, 'UTF-8');?>
">
    <img
      class="logo img-fluid"
      src="<?php echo htmlspecialchars($_smarty_tpl->tpl_vars['shop']->value['logo_details']['src'], ENT_QUOTES, 'UTF-8');?>
"
      alt="<?php echo htmlspecialchars($_smarty_tpl->tpl_vars['shop']->value['name'], ENT_QUOTES, 'UTF-8');?>
"
      loading="lazy"
      width="<?php echo htmlspecialchars($_smarty_tpl->tpl_vars['shop']->value['logo_details']['width'], ENT_QUOTES, 'UTF-8');?>
"
      height="<?php echo htmlspecialchars($_smarty_tpl->tpl_vars['shop']->value['logo_details']['height'], ENT_QUOTES, 'UTF-8');?>
">
  </a>
<?php
}}
/*/ smarty_template_function_renderLogo_162588955761a183016fa658_75833604 */
}
