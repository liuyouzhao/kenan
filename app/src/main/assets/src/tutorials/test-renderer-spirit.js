var resourceManager = {};
kenan_api_script.include("src/tutorials/test-render-graphics.js");

var i = 0;
var EN_EFFECT_STATE =
{
    GL2D_BLUR             : 17,   // blur effect
    GL2D_DARK             : 18,   // dark effect
    GL2D_BI               : 19,   // change to white black map
    GL2D_NORMAL_NOTEXTURE : 20,
    GL2D_NORMAL           : 21
};

var eff = orc2d.Effect_Load(EN_EFFECT_STATE.GL2D_NORMAL);

var spr = orc2d.Factory_CreateSpirit(-1, 0, 0, 1, 1, 32, 32, 1);
var spr2 = orc2d.Factory_CreateSpirit(-1, 0, 0, 1, 1, 64, 64, 1);

spr.SetHotSpot(0, 0);
spr2.SetHotSpot(0, 0);
spr.SetColor(1.0, 1.0, 1.0, 1.0, 0);
spr.SetColor(1.0, 1.0, 1.0, 1.0, 1);
spr.SetColor(1.0, 1.0, 1.0, 1.0, 2);
spr.SetColor(1.0, 1.0, 1.0, 1.0, 3);

spr2.SetColor(1.0, 1.0, 1.0, 1.0, 0);
spr2.SetColor(1.0, 1.0, 1.0, 1.0, 1);
spr2.SetColor(1.0, 1.0, 1.0, 1.0, 2);
spr2.SetColor(1.0, 1.0, 1.0, 1.0, 3);

var test = 0;

kenan_api_script.run("test = 1;");

log.info("kenan_api_script result: " + (test == 1));