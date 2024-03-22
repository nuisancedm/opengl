#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    MovementSpeed = 2.5f;
    MouseSensitivity = 0.1f;
    fov = 45.0f;
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
};

glm::mat4 Camera::GetViewMatrix() {
    //@@ glm::lookAt(Camera_Position, Camera_Target_Position, World_Up_Vector)
    return glm::lookAt(Position, Position + Front, Up);
};

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        Position += Front * velocity;
    }
    if (direction == BACKWARD) {
        Position -= Front * velocity;
    }
    if (direction == LEFT) {
        Position -= Right * velocity;
    }
    if (direction == RIGHT) {
        Position += Right * velocity;
    }
    if (direction == DOWN) {
        Position -= Up * velocity;
    }
    if (direction == UP) {
        Position += Up * velocity;
    }
};
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
};

void Camera::ProcessMouseScroll(float yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
};

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // recalculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
