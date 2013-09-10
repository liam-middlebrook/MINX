uniform float TIME_FROM_INIT;

varying vec2 texCoord;
varying vec3 lightDir,normal,halfVector;
varying vec4 ambient,diffuse,specular,ecPos;

void main()
{
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	ecPos = gl_Position;
	texCoord = gl_MultiTexCoord0.xy;
	lightDir = normalize(vec3(gl_LightSource[0].position-gl_Position));
	normal = normalize(gl_NormalMatrix * gl_Normal);
	halfVector = normalize(gl_LightSource[0].halfVector - gl_Position);
	ambient = gl_FrontMaterial.ambient*(gl_LightSource[0].ambient+gl_LightModel.ambient);
	diffuse = gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse;
	specular = gl_FrontMaterial.specular*gl_LightSource[0].specular;
        gl_TexCoord[0] = gl_MultiTexCoord0;
} 
