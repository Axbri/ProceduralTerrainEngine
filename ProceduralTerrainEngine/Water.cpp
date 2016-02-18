#include "Water.h"

Water::Water(Loader loader)
{
	float positions[] = { -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f };
	int indices[] = { 0, 1, 3, 3, 1, 2 };

	// create a ground-plane that is 2x2 meters and scale it so it becomes 100x100 meters. 
	waterplane = loader.createModel(positions, 12, indices, 6);

	Mat4 scaleMatrix;
	scaleMatrix.loadScale(150, 1, 150);
	waterplane.setModelMatrix(scaleMatrix);

	dudvTexture = loader.loadBMPtexture("water_dudv.bmp");
	normalTexture = loader.loadBMPtexture("water_nomal.bmp");
	shader.createShader("waterVertexShader.glsl", "waterFragmentShader.glsl");
	
	reflectionFrameBuffer = FrameBufferUtilities::createBuffer();
	reflectionTexture = FrameBufferUtilities::createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionDepthBuffer = FrameBufferUtilities::createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	FrameBufferUtilities::unbindCurrentBuffer();

	refractionFrameBuffer = FrameBufferUtilities::createBuffer();
	refractionTexture = FrameBufferUtilities::createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	refractionDepthTexture = FrameBufferUtilities::createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	FrameBufferUtilities::unbindCurrentBuffer();

	double waterheight = 0; 
	reflection_clip_plane = Vec4{ 0, 1, 0, -waterheight + 0.05 }; 
	refraction_clip_plane = Vec4{ 0, -1, 0, -waterheight };
}

Water::~Water()
{
	cleanUp(); 
}

void Water::cleanUp()
{
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
	shader.cleanUp(); 
}

void Water::bindReflectionBuffer()
{
	FrameBufferUtilities::bindBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void Water::bindRefractionBuffer()
{
	FrameBufferUtilities::bindBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void Water::render(GLFWwindow * window, Settings settings, Camera camera, vector<Light> allLights)
{
	Mat4 scale, translation;
	scale.loadScale(SIZE, 1, SIZE);
	translation.loadTranslation(camera.getPosition());
	waterplane.setModelMatrix(scale * translation);

	shader.start();
	shader.setUniformMat4("projectionMatrix", camera.getProjectionMatrix());
	shader.setUniformMat4("viewMatrix", camera.getViewMatrix());
	shader.setUniformMat4("modelMatrix", waterplane.getModelMatrix());
	Light::loadLightsToShader(shader, allLights);
	shader.setUniformInt("reflectionTexture", 0);
	shader.setUniformInt("refractionTexture", 1);
	shader.setUniformInt("depthTexture", 2);
	shader.setUniformInt("dudvTexture", 3);
	shader.setUniformInt("normalTexture", 4);
	shader.setUniformFloat("time", (float)glfwGetTime());
	shader.setUniformVec3("fogColor", settings.getFogColor());
	shader.setUniformFloat("fogDencity", settings.getFogDencity());
	shader.setUniformFloat("gamma", settings.getGamma());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(waterplane.get_id());
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, refractionDepthTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, normalTexture);

	glDrawElements(GL_TRIANGLES, waterplane.get_vertexcount(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	glDisable(GL_BLEND);

	shader.stop();
}

Vec4 Water::getReflectionClipPlane()
{
	return reflection_clip_plane;
}

Vec4 Water::getRefractionClipPlane()
{
	return refraction_clip_plane;
}
