#include "Entity.h"
#include <string>

Entity::Entity()
{
	position = glm::vec3(0);
	acceleration = glm::vec3(0);
	velocity = glm::vec3(0);

	modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
	if (isActive == false || other->isActive == false) {
		return false;
	}

	float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
	float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

	if (xdist < 0 && ydist < 0) return true;

	return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity* object = &objects[i];

		if (CheckCollision(object))
		{
			float ydist = fabs(position.y - object->position.y);
			float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
			if (velocity.y < 0) {
				//position.y += penetrationY;
				velocity.y = 0;
				if (object->type == 0) {
					collidedBottom = true;
				}
				else if (object->type == 1) {
					platformTouched = true;
				}
			}
		}
	}
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity* object = &objects[i];

		if (CheckCollision(object))
		{
			float xdist = fabs(position.x - object->position.x);
			float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
			if (velocity.x > 0) {
				velocity.x = 0;
				if (object->type == 0) {
					collidedBottom = true;
				}
				else if (object->type == 1) {
					platformTouched = true;
				}
			}
			else if (velocity.x < 0) {
				velocity.x = 0;
				if (object->type == 0) {
					collidedBottom = true;
				}
				else if (object->type == 1) {
					platformTouched = true;
				}
			}
		}
	}
}

void Entity::Update(float deltaTime, Entity* platforms, int platformCount)
{
	if (isActive == false) {
		return;
	}

	if (collidedBottom == false && collidedLeft == false && collidedRight == false
		&& platformTouched == false) {
		velocity.x += acceleration.x * deltaTime;
		velocity.y += acceleration.y * deltaTime;

		position.y += velocity.y * deltaTime; 
		CheckCollisionsY(platforms, platformCount);

		position.x += velocity.x * deltaTime;
		CheckCollisionsX(platforms, platformCount);
	}

	else {
		velocity = glm::vec3(0);
	}

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram* program) {
	if (isActive == false) {
		return;
	}

	program->SetModelMatrix(modelMatrix);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}