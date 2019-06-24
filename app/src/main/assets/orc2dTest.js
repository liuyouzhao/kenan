var gl = x.gl; 
var orc2d = x.orc2d;
var Orc2dSprite = x.Orc2dSprite;
var Image = x.Image;
var log_e = x.log_e;
var resource = x.res;

var ready = true;
var inited = false;

var MIN_OBJECTS	= 100;
var MAX_OBJECTS = 600;

var MAX_PARTICALS = 80;

var SCREEN_WIDTH  = 480;
var SCREEN_HEIGHT = 800;

var PI = 3.14159;

var img = new Image();
/*var imgarr = [];
for(var i = 0; i < 20; i ++) {
    imgarr[i] = new Image();
    imgarr[i].src = "http://hujia-photo-family001.oss-cn-hangzhou.aliyuncs.com/zazaka.png";
    imgarr[i].onload = function() {
        x.log_e("image onload: " + i);
    }
}*/

var particalImg = new Image();


var hTex = 0;
var hParticalTex = 0;

var soundReady = false;
this.onloadresource = function(id) {
    
    //x.log_e("Sound Onload ? : " + id + ", " + soundId);
    if(sounds[2] == id) {
        resource.PlaySound(sounds[2]);
        soundReady = true;
        //x.log_e("Sound Onload: " + soundId);
    }
    
}

var sounds = [];
sounds[0] = resource.LoadSoundResource("1.mp3");
sounds[1] = resource.LoadSoundResource("2.mp3");
sounds[2] = resource.LoadSoundResource("3.mp3");


function loadTexture() {
    x.log_e("image onload......");
    
    img.onload = function() {
        x.log_e("image onload");
        x.log_e("onloadjs 1");
        var pixels = img.getImageData();
        
        x.log_e("onloadjs 2");
        hTex = orc2d.Texture_Load(pixels, img.width, img.height);
        
        x.log_e("onloadjs 3");
        x.log_e('image onload ' + hTex + ", " + img.width + ", " + img.height); 
    }
    
    particalImg.onload = function() {
        x.log_e("image onload");
        x.log_e("onloadjs 1");
        var pixels = particalImg.getImageData();
        
        x.log_e("onloadjs 2");
        hParticalTex = orc2d.Texture_Load(pixels, particalImg.width, particalImg.height);
        
        x.log_e("onloadjs 3");
        x.log_e('image onload ' + hParticalTex + ", " + particalImg.width + ", " + particalImg.height); 
    }
    
    img.src = "http://hujia-photo-family001.oss-cn-hangzhou.aliyuncs.com/zazaka.png";
    particalImg.src = "http://hujia-photo-family001.oss-cn-hangzhou.aliyuncs.com/zazaka.png";
}

function reloadTexture() {
    var pixels = img.getImageData();
    hTex = orc2d.Texture_Load(pixels, img.width, img.height);
    
    pixels = particalImg.getImageData();
    hParticalTex = orc2d.Texture_Load(pixels, particalImg.width, particalImg.height);
}


var gl2EffectState =
{
    GL2D_BLUR             : 17,   // blur effect
    GL2D_DARK             : 18,   // dark effect
    GL2D_BI               : 19,   // change to white black map
    GL2D_NORMAL_NOTEXTURE : 20,
    GL2D_NORMAL           : 21
};

var pObjects = [];

var r = 0.1;

var eff = orc2d.Effect_Load(gl2EffectState.GL2D_NORMAL);

function reloadEffect() {
    eff = orc2d.Effect_Load(gl2EffectState.GL2D_NORMAL);
}

var spr;
var sprPar;

var parGroup = [];
var curnumPar = 0;
function fillParGourp(particals) {
    var insert = false;
    for(var i = 0; i < parGroup.length; i ++) {
        if(!parGroup[i]) {
            parGroup[i] = particals;
            return;
        }
    }
    parGroup[parGroup.length] = particals;
}

function createPars(cx, cy) {
    var partical_list = [];
    for(var i = 0; i < MAX_PARTICALS; i ++) {
        var parObject = 
        {
            x:0,
            y:0,
            dx:0,
            dy:0,
            px:0,
            py:0,
            scale:1.0,
            rot:0.0,
            dscale:0.0,
            drot:0.0,
            tx1:0.0,
            ty1:0.0,
            tx2:1.0,
            ty2:1.0,
            colorR:1.0,
            colorG:1.0,
            colorB:1.0,
            colorA:1.0
        };
        partical_list[i] = parObject;
	    partical_list[i].x = cx;
	    partical_list[i].y = cy;
	    partical_list[i].dx = (Math.random() - 0.5)*50;
	    partical_list[i].dy = (Math.random() - 0.5)*50;
	    partical_list[i].scale = Math.random() * 1.5 + 0.5;
	    partical_list[i].dscale = (Math.random() - 0.5) * 2 * 0.1;
	    partical_list[i].rot = Math.random() * PI * 2;
        partical_list[i].tx1 = 0.0;
        partical_list[i].ty1 = 0.0;
        partical_list[i].tx2 = 1.0;
        partical_list[i].ty2 = 1.0;
	    //partical_list[i].tx1 = (i % 4) / 4.0;
	    //partical_list[i].ty1 = (i % 4) / 4.0;
	    //partical_list[i].tx2 = partical_list[i].tx1 + 0.25;
	    //partical_list[i].ty2 = partical_list[i].ty1 + 0.25;
	    //pObjects[i].drot = (Math.random() - 0.5) * 2 * 0.1;
    }
    return partical_list;
}

var init = function() {
    spr = new Orc2dSprite(hTex, 0, 0, 1, 1, 64, 64, 1);
    spr.SetHotSpot(32, 32);
    
    sprPar = new Orc2dSprite(hParticalTex, 0, 0, 1, 1, 32, 32, 1);
    sprPar.SetHotSpot(16, 16);
    
    for(var i=0; i < MAX_OBJECTS; i++) {
        var sprObject = 
        {
	        x:0,
	        y:0,
            dx:0,
            dy:0,
            px:0,
            py:0,
	        scale:1.0,
	        rot:0.0,
	        dscale:0.0,
	        drot:0.0,
	        colorR:1.0,
	        colorG:1.0,
	        colorB:1.0,
	        colorA:1.0
        };
		pObjects[i] = sprObject;
		pObjects[i].x = 0;
		pObjects[i].y = 0;
		pObjects[i].x = Math.random() * SCREEN_WIDTH;
		pObjects[i].y = Math.random() * SCREEN_HEIGHT;
		pObjects[i].dx = (Math.random() - 0.5)*2;
	    pObjects[i].dy = (Math.random() - 0.5)*2;
		pObjects[i].scale = Math.random() * 1.5 + 0.5;
		pObjects[i].dscale = (Math.random() - 0.5) * 2 * 0.1;
		pObjects[i].rot = Math.random() * PI * 2;
		pObjects[i].drot = (Math.random() - 0.5) * 2 * 0.1;
        var rand = Math.random() * 100;
        
        var sprColors =
        [
	        0xFFFFFFFF, 0xFFFFE080, 0xFF80A0FF, 0xFFA0FF80, 0xFFFF80A0
        ];
        
        var sprFloatColors = [];
        
        for(var j = 0; j < sprColors.length; j ++) {
            var color = sprColors[j];
            var a = ((color & 0xFF000000) >> 24) / 255;
            var r = ((color & 0x00FF0000) >> 16) / 255;
            var g = ((color & 0x0000FF00) >> 8) / 255;
            var b = ((color & 0x000000FF)) / 255;
            sprFloatColors[j] = {};
            sprFloatColors[j].a = a;
            sprFloatColors[j].r = r;
            sprFloatColors[j].g = g;
            sprFloatColors[j].b = b;
        }

		if(rand < 20) {
		    pObjects[i].colorR = sprFloatColors[0].r;
		    pObjects[i].colorG = sprFloatColors[0].g;
		    pObjects[i].colorB = sprFloatColors[0].b;
		}
		else if(rand < 40){
		    pObjects[i].colorR = sprFloatColors[1].r;
		    pObjects[i].colorG = sprFloatColors[1].g;
		    pObjects[i].colorB = sprFloatColors[1].b;
		}
		else if(rand < 60){
		    pObjects[i].colorR = sprFloatColors[2].r;
		    pObjects[i].colorG = sprFloatColors[2].g;
		    pObjects[i].colorB = sprFloatColors[2].b;
		}
		else if(rand < 80){
		    pObjects[i].colorR = sprFloatColors[3].r;
		    pObjects[i].colorG = sprFloatColors[3].g;
		    pObjects[i].colorB = sprFloatColors[3].b;
		}
		else {
		    pObjects[i].colorR = sprFloatColors[4].r;
		    pObjects[i].colorG = sprFloatColors[4].g;
		    pObjects[i].colorB = sprFloatColors[4].b;
		}
		pObjects[i].colorA = 0.7;
	}
}

var FrameFunc = function() {
    var dt = 1.0;
	// Update the scene
	for(var i = 0;i < MAX_OBJECTS; i++) {
		pObjects[i].x+=pObjects[i].dx*dt + pObjects[i].px*dt;

		if(pObjects[i].x>SCREEN_WIDTH || pObjects[i].x<0)
        {
            if(pObjects[i].x > SCREEN_WIDTH)
            {
                pObjects[i].x = SCREEN_WIDTH;
            }
            else if(pObjects[i].x < 0)
            {
                pObjects[i].x = 0;
            }
            pObjects[i].dx=-pObjects[i].dx;
            pObjects[i].px = 0;
        }

		pObjects[i].y+=pObjects[i].dy*dt + pObjects[i].py*dt;

		if(pObjects[i].y>SCREEN_HEIGHT || pObjects[i].y<0)
        {
            if(pObjects[i].y > SCREEN_HEIGHT)
            {
                pObjects[i].y = SCREEN_HEIGHT;
            }
            else if(pObjects[i].y < 0)
            {
                pObjects[i].y = 0;
            }
            pObjects[i].dy=-pObjects[i].dy;
            pObjects[i].py = 0;
        }

		pObjects[i].scale+=pObjects[i].dscale*dt;

		if(pObjects[i].scale>2 || pObjects[i].scale<0.5)
            pObjects[i].dscale=-pObjects[i].dscale;

		pObjects[i].rot+=pObjects[i].drot*dt;
		
		if(pObjects[i].px != 0.0) {
		    if(pObjects[i].px > 0.0) {
		        pObjects[i].px -= dt * 1;
		        if(pObjects[i].px <= 0.0) {
		            pObjects[i].px = 0.0;
		        }
		    }
		    else {
		        pObjects[i].px += dt * 1;
		        if(pObjects[i].px >= 0.0) {
		            pObjects[i].px = 0.0;
		        }
		    }
		}
		
		if(pObjects[i].py != 0.0) {
		    if(pObjects[i].py > 0.0) {
		        pObjects[i].py -= dt * 1;
		        if(pObjects[i].py <= 0.0) {
		            pObjects[i].py = 0.0;
		        }
		    }
		    else {
		        pObjects[i].py += dt * 1;
		        if(pObjects[i].py >= 0.0) {
		            pObjects[i].py = 0.0;
		        }
		    }
		}

	}

    for(var i = 0; i < parGroup.length; i ++) {
        if(parGroup[i]) {
            var pars = parGroup[i];
            var alive = pars.length;
            for(var j = 0; j < pars.length; j ++) {
                var p = pars[j];
                p.x+=p.dx*dt + p.px*dt;
		        p.y+=p.dy*dt + p.py*dt;
		        p.colorA -= 0.02;
		        p.colorA = p.colorA <= 0.0 ? 0.0 : p.colorA;
		        if(p.colorA == 0.0) {
		            alive --;
		        }
            }      
            if(alive == 0) {
                parGroup[i] = null;
            }   
        }
    }
}

var RenderFunc = function() {
    gl.clearColor(1.0, 1.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    
    //orc2d.Transform_rotate(r);
    //orc2d.Transform_translate(200, 200);
    //orc2d.Gfx_RenderQuadEx(-50, -50, -50, 50, 50, 50, 50, -50, 0, 255, 255, 255, 1, 0);
    
    orc2d.Effect_Active(eff, true);
    
    for(i = 0;i < MAX_OBJECTS; i++) {
		spr.SetColor(pObjects[i].colorR, pObjects[i].colorG, pObjects[i].colorB, pObjects[i].colorA, 0); 
		spr.SetColor(pObjects[i].colorR, pObjects[i].colorG, pObjects[i].colorB, pObjects[i].colorA, 1); 
		spr.SetColor(pObjects[i].colorR, pObjects[i].colorG, pObjects[i].colorB, pObjects[i].colorA, 2); 
		spr.SetColor(pObjects[i].colorR, pObjects[i].colorG, pObjects[i].colorB, pObjects[i].colorA, 3); 
		spr.SetTexture(hTex);
		spr.RenderEx(pObjects[i].x, pObjects[i].y, pObjects[i].rot, pObjects[i].scale);
	}
	
	for(var i = 0; i < parGroup.length; i ++) {
        if(parGroup[i]) {
            var pars = parGroup[i];
            for(var j = 0; j < pars.length; j ++) {
                var p = pars[j];
                sprPar.SetColor(p.colorR, p.colorG, p.colorB, p.colorA, 0); 
                sprPar.SetColor(p.colorR, p.colorG, p.colorB, p.colorA, 1); 
                sprPar.SetColor(p.colorR, p.colorG, p.colorB, p.colorA, 2); 
                sprPar.SetColor(p.colorR, p.colorG, p.colorB, p.colorA, 3);
                sprPar.SetTexture(hParticalTex);
                sprPar.SetUV(p.tx1, p.ty1, p.tx2, p.ty2);
                sprPar.RenderEx(p.x, p.y, p.rot, p.scale);
            }
        }
    }
	//spr.SetColor(1.0, 0.0, 0.0, 1.0, 0);
	//spr.SetColor(1.0, 0.0, 0.0, 1.0, 1);
	//spr.SetColor(1.0, 0.0, 0.0, 1.0, 2);
	//spr.SetColor(1.0, 0.0, 0.0, 1.0, 3);
    //spr.RenderEx(100, 100, 0.0, 1.0, 1.0);
    orc2d.Gfx_SwapBuffer();
    gl.flush();
}

this.update = function() {
    if(!ready)
        return;
    if(!inited) {
        init();
        inited = true;
    }
    FrameFunc();
    RenderFunc();
    
}; 

this.resume = function() {
    log_e("javascript resume");
    reloadTexture();
    reloadEffect();
    spr.SetTexture(hTex);
    ready = true;
}

this.ontouchstart = function(ex, ey) {
    log_e("ontouchstart ..." + ex + ", " + ey);
    for(var i = 0;i < MAX_OBJECTS; i++) {
        var x = pObjects[i].x;
        var y = pObjects[i].y;
        var d2 = (x-ex)*(x-ex) + (y-ey)*(y-ey);
        var dx = (x - ex) / Math.sqrt(d2);
        var dy = (y - ey) / Math.sqrt(d2);
        if(Math.sqrt(d2) <= 100) {
            pObjects[i].opx = pObjects[i].px;
            pObjects[i].opy = pObjects[i].py;
            pObjects[i].px = dx * 20;
		    pObjects[i].py = dy * 20;

        }
	}
	
	if(soundReady == true) {
        var id = (Math.random() * 100) % 3;
    	resource.PlaySound(sounds[parseInt(id)]);
	}
	
	var pars = createPars(ex, ey);
	fillParGourp(pars);
}

this.ontouchmove = function(ex, ey) {
    log_e("ontouchmove ...");
}

this.ontouchend = function(ex, ey) {
    log_e("ontouchend ...");
}



loadTexture();
x.log_e('code is ok'); 
