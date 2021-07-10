#include "../ViewController_Trackball.h"
#include "../Camera.h"

namespace Cool {

void ViewController_Trackball::look_at_the_origin(Camera& camera) {
	camera.translate(-get_look_at(camera));
}

glm::vec3 ViewController_Trackball::get_look_at(Camera const& camera) const {
	return camera.position() + _dist_to_look_at * camera.front_axis();
}

} // namespace Cool