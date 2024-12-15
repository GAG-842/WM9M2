#pragma once
#include "mesh.h"
#include "camera.h"
struct Bone
{
	std::string name;
	Matrix offset;
	int parentIndex;
};
struct Skeleton
{
	std::vector<Bone> bones;
	Matrix globalInverse;
};
struct AnimationFrame
{

	std::vector<Vec3> positions;
	std::vector<Quaternion> rotations;
	std::vector<Vec3> scales;
};

class AnimationSequence
{
	// stores animation data for one animation
public:
	std::vector<AnimationFrame> frames; // transformation data for bones
	float ticksPerSecond; // speed of animation

	Vec3 interpolate(Vec3 p1, Vec3 p2, float t) {
		return ((p1 * (1.0f - t)) + (p2 * t)); // linear interpolation
	}
	Quaternion interpolate(Quaternion q1, Quaternion q2, float t) {
		return Quaternion::slerp(q1, q2, t); // ensure smooth rotation
	}
	float duration() {
		return ((float)frames.size() / ticksPerSecond);// total duration of animation sequence
	}

	void calcFrame(float t, int& frame, float& interpolationFact)
	{
		// find frame given time
		interpolationFact = t * ticksPerSecond;
		frame = (int)floorf(interpolationFact);
		interpolationFact = interpolationFact - (float)frame; // fractional part
		frame = min(frame, frames.size() - 1);
	}

	int nextFrame(int frame)
	{
		// find next frame, returns index for next frame
		return min(frame + 1, frames.size() - 1);
	}

	Matrix interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex) {
		//Interpolates transformations for a specific bone and calculates its global transformation.
		int nextFrameIndex = nextFrame(baseFrame);
		Matrix temp;
		// interpolate scale, rotation and translation
		Matrix scale = temp.Scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrameIndex].scales[boneIndex], interpolationFact));
		Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrameIndex].rotations[boneIndex], interpolationFact).toMatrix();
		Matrix translation = temp.Translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrameIndex].positions[boneIndex], interpolationFact));
		Matrix local = scale * rotation * translation;
		if (skeleton->bones[boneIndex].parentIndex > -1) {
			Matrix global = local * matrices[skeleton->bones[boneIndex].parentIndex];
			return global;
		}
		return local;
	}


};

class Animation
{
	// store skeleton and animations
public:
	std::map<std::string, AnimationSequence> animations; // map of animation sequences
	Skeleton skeleton; // The skeleton structure defining bone hierarchy and transformations.

	int boneSize() {
		int boneSize = skeleton.bones.size();
		return boneSize;
	}
	// Calculation of the current frame and interpolation factor
	void calcFrame(std::string name, float t, int& frame, float& interpolationFact) {
		animations[name].calcFrame(t, frame, interpolationFact);
	}
	// bone transformation interpolation to the corresponding AnimationSequence.
	Matrix interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
		return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
	}

	void calcFinalTransforms(Matrix* matrices)
	{
		for (int i = 0; i < boneSize(); i++)
		{
			matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse;
		}
	}
};

class AnimationInstance
{
public:
	Animation* animation;
	std::string currentAnimation;
	Matrix matrices[256];
	float t;

	void resetAnimationTime()
	{
		t = 0;
	}
	bool animationFinished()
	{
		if (t > animation->animations[currentAnimation].duration())
		{
			return true;
		}
		return false;
	}

	void update(std::string name, float dt) {
		if (name == currentAnimation) {
			t += dt;
		}
		else {
			currentAnimation = name;  t = 0;
		}
		if (animationFinished() == true) { resetAnimationTime(); }
		int frame = 0;
		float interpolationFact = 0;
		animation->calcFrame(name, t, frame, interpolationFact);
		for (int i = 0; i < animation->boneSize(); i++)
		{
			matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
		}
		animation->calcFinalTransforms(matrices);

	}

};

class AnimationPlayer{
public:
	std::vector<mesh> meshes;
	AnimationInstance instance;
	Animation animation1;
	TextureManager textures;
	std::vector<std::string> textureFilenames;
	Vec3 position = Vec3(0.f, 0.f, 0.f); // 目标人物的位置
	float speed = 0.05f;

	void init(DxCore* core, std::string filename) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);

		for (int i = 0; i < gemmeshes.size(); i++) {
			mesh mesh;
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				vertices.push_back(v);
			}
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			// Load texture with filename: gemmeshes[i].material.find("diffuse").getValue()
			textures.load(core, gemmeshes[i].material.find("diffuse").getValue());
			mesh.init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}
		for (int i = 0; i < gemanimation.bones.size(); i++)
		{
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation1.skeleton.bones.push_back(bone);
		}
		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			std::string name = gemanimation.animations[i].name;
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
			for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
			{
				AnimationFrame frame;
				for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
				{
					Vec3 p;
					Quaternion q;
					Vec3 s;
					memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
					frame.positions.push_back(p);
					memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
					frame.rotations.push_back(q);
					memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
					frame.scales.push_back(s);
				}
				aseq.frames.push_back(frame);
			}
			animation1.animations.insert({ name, aseq });
		}
		instance.animation = &animation1;
	}
	void handleMovement(camera& camera) {
		position.x = camera.position.x;
		position.y = camera.position.y - 1.5;
		position.z = camera.position.z - 0.3;
	}

	void draw(shader* shaders, DxCore* core,  float dt, camera* camera, Window* canvas) {
		Matrix sm;
		Matrix tm;
		sm.Scaling(Vec3(0.01f, 0.01f, 0.01f));
		tm.Translation(position);
		Matrix rm; //cannot work
		rm.RotationY(position, camera->yaw);
		Matrix worldPlane = sm * tm * rm; 
		if (canvas->keyPressed('W')){ instance.update("Running", dt); }
		if (canvas->keyPressed('A')) { instance.update("Running", dt); }
		if (canvas->keyPressed('S')) { instance.update("Running", dt); }
		if (canvas->keyPressed('D')) { instance.update("Running", dt); }
		else { instance.update("idle", dt); }

		shaders->updateConstantVS("MeshBuffer", "W", &worldPlane);
		//shaders->updateConstantVS("MeshBuffer", "VP", &vp);
		shaders->updateConstantVS("MeshBuffer", "bones", instance.matrices);
		shaders->apply(core);
		for (int i = 0; i < meshes.size(); i++)
		{
			shaders->updateTexturePS(core, "tex", textures.find(textureFilenames[i]));

			meshes[i].draw(core);
		}
	}
	Vec3 getPosition() const {
    return position; // 返回人物的当前位置
	}
};

class AnimationDinosaur {
public:
	std::vector<mesh> meshes;
	AnimationInstance instance;
	Animation animation1;
	TextureManager textures;
	std::vector<std::string> textureFilenames;
	Vec3 position = Vec3(35.f, 0.f, 35.f); 
	float speed = 0.05f;

	void init(DxCore* core, std::string filename) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);

		for (int i = 0; i < gemmeshes.size(); i++) {
			mesh mesh;
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				vertices.push_back(v);
			}
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			// Load texture with filename: gemmeshes[i].material.find("diffuse").getValue()
			textures.load(core, gemmeshes[i].material.find("diffuse").getValue());
			mesh.init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}
		for (int i = 0; i < gemanimation.bones.size(); i++)
		{
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation1.skeleton.bones.push_back(bone);
		}
		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			std::string name = gemanimation.animations[i].name;
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
			for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
			{
				AnimationFrame frame;
				for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
				{
					Vec3 p;
					Quaternion q;
					Vec3 s;
					memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
					frame.positions.push_back(p);
					memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
					frame.rotations.push_back(q);
					memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
					frame.scales.push_back(s);
				}
				aseq.frames.push_back(frame);
			}
			animation1.animations.insert({ name, aseq });
		}
		instance.animation = &animation1;
	}

	void draw(shader* shaders, DxCore* core, float dt, Vec3 cp) {
		Matrix op;
		Matrix worldPlane = op.Translation(position);
		if (position.getLength() - cp.getLength() < 10.f) { instance.update("attack", dt); }
		else{ instance.update("Idle", dt); }
		
		shaders->updateConstantVS("MeshBuffer", "W", &worldPlane);
		//shaders->updateConstantVS("MeshBuffer", "VP", &vp);
		shaders->updateConstantVS("MeshBuffer", "bones", instance.matrices);
		shaders->apply(core);
		for (int i = 0; i < meshes.size(); i++)
		{
			shaders->updateTexturePS(core, "tex", textures.find(textureFilenames[i]));

			meshes[i].draw(core);
		}
	}
};
