var i = 0;
var EN_EFFECT_STATE =
{
    GL2D_BLUR             : 17,   // blur effect
    GL2D_DARK             : 18,   // dark effect
    GL2D_BI               : 19,   // change to white black map
    GL2D_NORMAL_NOTEXTURE : 20,
    GL2D_NORMAL           : 21
};

var eff = orc2d.Effect_Load(EN_EFFECT_STATE.GL2D_NORMAL_NOTEXTURE);

var Clz = function()
{
    var a = 0;
    var b = 0;
}
var spr = orc2d.Factory_CreateSpirit(-1, 0, 0, 1, 1, 64, 64, 1);
var spr2 = orc2d.Factory_CreateSpirit(-1, 0, 0, 1, 1, 64, 64, 1);
spr.SetHotSpot(32, 32);
spr2.SetHotSpot(32, 32);

var x = 400;
var y = 400;

this.update = function() {

    orc2d.Gfx_SetClearColor(0.2, 0.2, 0.2, 1.0);
    orc2d.Gfx_Clear();
    orc2d.Effect_Active(eff, true);

    spr.SetColor(1.0, 0.0, 0.0, 1.0, 0);
    spr.SetColor(1.0, 0.0, 0.0, 1.0, 1);
    spr.SetColor(1.0, 0.0, 0.0, 1.0, 2);
    spr.SetColor(1.0, 0.0, 0.0, 1.0, 3);
    spr.RenderEx(x, y, 1.47, 1.0);

    x ++;
    if(x >= 500)
    {
        x = 0;
    }

    spr2.SetColor(1.0, 0.0, 0.0, 1.0, 0);
    spr2.SetColor(1.0, 0.0, 0.0, 1.0, 1);
    spr2.SetColor(1.0, 0.0, 0.0, 1.0, 2);
    spr2.SetColor(1.0, 0.0, 0.0, 1.0, 3);
    spr2.RenderEx(x, y + 200, 1.47, 1.0);


    spr2.RenderEx(400, 200, 1.47, 1.0);

    spr2.RenderEx(100, 400, 1.47, 1.0);
    orc2d.Gfx_SwapBuffer();
}
