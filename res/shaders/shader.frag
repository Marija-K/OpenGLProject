#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
        // linearly interpolate between both textures (80% container, 20% awesomeface)
        //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
		/*for (int i = 0; i<100 ; i++){
			if(i%2==0){
				FragColor = texture(texture1, TexCoord);
			}
			else {
				FragColor = texture(texture2, TexCoord);
			}
		}*/
		FragColor = texture(texture1, TexCoord);
		//FragColor = texture(texture2, TexCoord);
}
