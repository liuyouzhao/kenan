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

log.info("-----1");
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

var hTex = 0;
var hTex2 = 0;
log.info("-----1.1");
var img = orc2d.Resource_CreateImage("img_sword");
var img2 = orc2d.Resource_CreateImage("img_volcano");

log.info("-----1.2   " + img);
var x = 0;
var y = 0;
log.info("-----1.3");
var resourceManager = {};

log.info("-----1.4");

img.onload = function() {
    log.info("image1's onload");
    var pixels = img.getImageData();
    //log.info("imgdata:" + pixels);
    if(hTex == 0 && pixels) {
        hTex = orc2d.Texture_Load(pixels, img.width, img.height);
        spr.SetTexture(hTex);
    }
    spr.Resize(img.width, img.height);
}

img2.onload = function() {
    log.info("image2's onload");
    var pixels = img2.getImageData();
    //log.info("imgdata:" + pixels);
    if(hTex2 == 0 && pixels) {
        hTex2 = orc2d.Texture_Load(pixels, img2.width, img2.height);
        spr2.SetTexture(hTex2);
    }
    spr2.Resize(img2.width, img2.height);
}


var pic1 = "data:image/png;base64,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";
//var pic1 = "x_cache.png";
var pic2 = "x_cache.png";

resourceManager["img_sword"] = img;
resourceManager["img_volcano"] = img2;
img.src = pic2;
img2.src = pic1;

this.onload = function(mark) {
    log.info("onload triggered " + mark);
    var img = resourceManager[mark];
    img.onload();
}

this.update = function() {

    orc2d.Gfx_SetClearColor(0.2, 0.2, 0.2, 1.0);
    orc2d.Gfx_Clear();
    orc2d.Effect_Active(eff, true);

    spr.RenderEx(x, y, 1.47, 1.0);

    spr2.RenderEx(x, y + 200, 0, 1.0);


    spr2.RenderEx(400, 200, 1.47, 1.0);

    spr2.RenderEx(100, 400, 1.47, 1.0);
    x ++;
    if(x >= 500)
    {
        x = 0;
    }

    orc2d.Gfx_SwapBuffer();
}