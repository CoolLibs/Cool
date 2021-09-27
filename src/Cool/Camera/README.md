# Camera

## The camera, the controllers and the manager

## The controllers

### Orbital

This is a family of controllers who orbit around a given center (which can be translated). (They are not seperate classes because we want them to retain the same center of rotation as we switch between them ; + they don't have specific data, only behaviour)

#### Trackball

This is the simplest model that you will see in many tutorials ; it rotates around the world's up axis, and the camera's right axis.

#### Turntable

It is a simple twist on the trackball concept : if your head is upside down then the rotation around the up axis is inverted so that it feels more natural. I took this idea from Blender, where turntable is the default camera controller. *(A small difference to note : in Blender the test of "upsidedown-ness" is done once when you start dragging with the mouse, whereas we update this check every frame while you are dragging. I am note sure which one I prefer and would happily take any feedback on that.)*

#### AxisFree

When working on scenes that don't have a clear up axis (like a sphere) then trackball and turntable don't really make sense and can feel constraining. This is why we introduce *AxisFree* whose rotation only depends on the current orientation of the camera and not on an absolute up axis (we use the camera's up axis instead of the world's one).

### Freefly

## JIT Dependencies

## Stateless view controllers

To allow us to switch controllers easily, they must not have a state representing what the current camera should be (also, this violates *Single Source of Truth*).

We used to have

```cpp
class ViewController_Trackball {
public:
      /*...*/
      // Applies the state of the controller to the camera
      void update_transform_matrix_of(Camera& camera);
private:
      // State of the controller
      glm::vec3 _look_at         = glm::vec3(0.0f);
      float     _dist_to_look_at = 3.0f;
      float     _angle_ground    = 0.0f;
      float     _angle_up        = 0.0f;
      /*...*/
};
```

but what happens if we switch from this controller to a freefly one, move the camera around a bit, and then come back to this controller ? We would need to update the state of the controller to match the new one of the camera (otherwise the next call to ```update_transform_matrix_of(camera)``` would undo the movements we did with the freefly controller). This is a bit of a mess to manage ! (as always when *Single Source of Truth* is violated).

This is why the controllers should be stateless and apply transformations directly to the transform matrix of the camera.

## ```Camera&``` or ```Camera const&``` ?

Some actions like 
```cpp
class ITrackballState {
      /*...*/
      virtual void on_wheel_up(ViewController_Trackball& controller, Camera& camera);
};
```

actually never modify the camera, no matter the state we are in. So why don't we use ```Camera const&``` instead of ```Camera&``` ? That is because *conceptually* it could. What we mean by that is that a user of this interface should accept that every user input like ```on_wheel_up``` *could* modify the camera. Even though the current implementation does not, we could make a change in the future that would. By making the camera reference non-const, we state that this is a possibility and that users shouldn't rely on the fact that ```on_wheel_up``` will not modify the camera.