#ifndef __ORBITFOLLOW2D_H_
#define __ORBITFOLLOW2D_H_

#include <Godot.hpp>
#include <Path2D.hpp>
#include <Area2D.hpp>
#include <Curve2D.hpp>
#include <Sprite.hpp>
#include <PathFollow2D.hpp>
#include <PhysicsBody2D.hpp>

namespace godot {

class OrbitPath2D : public Path2D {
    GODOT_CLASS(OrbitPath2D, Sprite
    )

private:
    // User Defined
    float semi_major_axis;
    float eccentricity;
    float argument_of_periapsis;
    float draw_resolution;
    Color draw_color;

    // Memoizations
    float _semi_minor_axis;
    // float _cached_angular_velocity;
    // float _standard_gravitational_parameter;
    // PhysicsBody2D *_body;
    // PathFollow2D *_path_follow;
    // Area2D *_area;

public:
    static void _register_methods();

    OrbitPath2D();

    ~OrbitPath2D();

    void _init();
    void _draw();

    // Important Functions
    void generate_path();

    void draw_ellipse();

    // Setters
    void set_semi_major_axis(const float value);

    void set_eccentricity(const float value);

    void set_argument_of_periapsis(const float value);

    void set_draw_resolution(const int value);

    void set_draw_color(const Color value);

    //  void set_gravity(const float value);

    // Getters
    float get_semi_major_axis();

    float get_eccentricity();

    float get_argument_of_periapsis();

    int get_draw_resolution();

    Color get_draw_color();

    // float get_gravity();

    // Memoized Getters
    float get_semi_minor_axis();

    // Vector2 get_velocity();

    Vector2 get_focus_point();

    // PhysicsBody2D *get_body();

    // PathFollow2D *get_path_follow();

    // Area2D *get_area();

    // float get_standard_gravitational_parameter();
};

}

#endif // __ORBITFOLLOW2D_H_
