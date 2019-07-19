texture glowTexture;

float4x4 texTrans : WorldProjection;
float pSize;

sampler GlowSampler = sampler_state
{
    Texture   = (glowTexture);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


//Passed to the vertex shader from the pipeline
struct GLOW_INPUT
{
	float4 pos : POSITION;
	float2 texCoord : TEXCOORD;
};

//VS output / PS input:
struct GLOW_OUTPUT
{
    float4 pos : POSITION;
    float2 texCoord0 : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
};

struct TEXTURE_OUTPUT
{
    float4 pos : POSITION;
    float2 texCoord0 : TEXCOORD0;
};
//PS output:
struct pixel
{
	float4 color : COLOR;
};

GLOW_OUTPUT glowVSHorizontal1(GLOW_INPUT IN)
{
	GLOW_OUTPUT OUT;
	
	OUT.pos =  mul( IN.pos, texTrans );
	OUT.texCoord0 = IN.texCoord + float2(-pSize*3, 0); 
	OUT.texCoord1 = IN.texCoord + float2(-pSize*2, 0); 
	OUT.texCoord2 = IN.texCoord + float2(-pSize*1, 0); 
	OUT.texCoord3 = IN.texCoord + float2(0, 0 ); 
	
	return OUT;
}
GLOW_OUTPUT glowVSHorizontal2(GLOW_INPUT IN)
{
	GLOW_OUTPUT OUT;
	
	OUT.pos =  mul( IN.pos, texTrans );
	OUT.texCoord0 = IN.texCoord + float2(pSize*3,0); 
	OUT.texCoord1 = IN.texCoord + float2(pSize*2, 0); 
	OUT.texCoord2 = IN.texCoord + float2(pSize*1, 0); 
	OUT.texCoord3 = IN.texCoord + float2(0, 0 ); 
	
	return OUT;
}
GLOW_OUTPUT glowVSVertical1(GLOW_INPUT IN)
{
	GLOW_OUTPUT OUT;
	
	OUT.pos =  mul( IN.pos, texTrans );
	OUT.texCoord0 = IN.texCoord + float2(0,-pSize*3); 
	OUT.texCoord1 = IN.texCoord + float2(0,-pSize*2); 
	OUT.texCoord2 = IN.texCoord + float2(0,-pSize*1); 
	OUT.texCoord3 = IN.texCoord + float2(0,0); 
	
	return OUT;
}
GLOW_OUTPUT glowVSVertical2(GLOW_INPUT IN)
{
	GLOW_OUTPUT OUT;
	
	OUT.pos =  mul( IN.pos, texTrans );
	OUT.texCoord0 = IN.texCoord + float2(0,pSize*3); 
	OUT.texCoord1 = IN.texCoord + float2(0,pSize*2);  
	OUT.texCoord2 = IN.texCoord + float2(0,pSize*1); 
	OUT.texCoord3 = IN.texCoord + float2(0,0);  
	
	return OUT;
}

TEXTURE_OUTPUT outputGlowVS(GLOW_INPUT IN)
{
	TEXTURE_OUTPUT OUT;
	OUT.pos =  mul( IN.pos, texTrans );
	OUT.texCoord0 = IN.texCoord;
	
	return OUT;
}

pixel glowPS(GLOW_OUTPUT IN)
{
	pixel OUT;	
	
	float4 color = tex2D( GlowSampler, IN.texCoord0 ) * 0.1;
	color += tex2D( GlowSampler, IN.texCoord1 ) * 0.3;
	color += tex2D( GlowSampler, IN.texCoord2 ) * 0.4;
	color += tex2D( GlowSampler, IN.texCoord3 ) * 0.25;
	
	OUT.color = color;
	OUT.color.a = 1.0f;
	
	return OUT;
}

pixel outputGlowPS(TEXTURE_OUTPUT IN)
{
	pixel OUT;
	OUT.color = tex2D( GlowSampler, IN.texCoord0 );	
	return OUT;
} 

technique T0
{
	pass P0 <string renderStage="texture";>
	{
		Sampler[0] = (GlowSampler);
		vertexshader = compile vs_1_1 glowVSHorizontal1();
		pixelshader  = compile ps_1_1 glowPS();
		fvf = XYZ | Tex1;
	}
	
	pass P1 <string renderStage="texture";>
	{
		Sampler[0] = (GlowSampler);
		vertexshader = compile vs_1_1 glowVSHorizontal2();
		pixelshader  = compile ps_1_1 glowPS();
		fvf = XYZ | Tex1;
	}
	
	pass P2 <string renderStage="texture";>
	{
		Sampler[0] = (GlowSampler);
		vertexshader = compile vs_1_1 glowVSVertical1();
		pixelshader  = compile ps_1_1 glowPS();
		fvf = XYZ | Tex1;
	}

	pass P3 <string renderStage="texture";>
	{
		Sampler[0] = (GlowSampler);
		vertexshader = compile vs_1_1 glowVSVertical2();
		pixelshader  = compile ps_1_1 glowPS();
		fvf = XYZ | Tex1;
	}

	pass P4 <string renderStage="post";>
	{
		Sampler[0] = (GlowSampler);
		vertexshader = compile vs_1_1 outputGlowVS();
		pixelshader  = compile ps_1_1 outputGlowPS();
		fvf = XYZ | Tex1;
		
		AlphaBlendEnable = true;
		BlendOp = Max;
		SrcBlend = One;
		DestBlend = One;
	}	
}

