#include "OrbitPath2D.hpp"
#include "orbits.hpp"

using namespace godot;

// TODO
void OrbitPath2D::_register_methods() {
    register_method("_draw", &OrbitPath2D::_draw);
    register_method("_init", &OrbitPath2D::_init);
    register_property<OrbitPath2D, float>("semi_major_axis", &OrbitPath2D::set_semi_major_axis, &OrbitPath2D::get_semi_major_axis, 10.0);
    register_property<OrbitPath2D, float>("eccentricity", &OrbitPath2D::set_eccentricity, &OrbitPath2D::get_eccentricity, 0.0);
    register_property<OrbitPath2D, float>("argument_of_periapsis", &OrbitPath2D::set_argument_of_periapsis, &OrbitPath2D::get_argument_of_periapsis, 0.0);
    register_property<OrbitPath2D, int>("draw_resolution", &OrbitPath2D::set_draw_resolution, &OrbitPath2D::get_draw_resolution, 100);
    register_property<OrbitPath2D, Color>("draw_color", &OrbitPath2D::set_draw_color, &OrbitPath2D::get_draw_color, godot::Color(0,0,0));
}

OrbitPath2D::OrbitPath2D() {}
OrbitPath2D::~OrbitPath2D() {}

// Godot functions
void OrbitPath2D::_init() {
    semi_major_axis = 10.0;
    _semi_minor_axis = 10.0;
    eccentricity = 0.0;  // Starts as a circle
    argument_of_periapsis = 0.0;
    draw_resolution = 100;
    draw_color = Color(0,0,0);
}
//void OrbitPath2D::_ready() {}

//void OrbitPath2D::_physics_process(float delta) {
//	_path_follow->set_offset(_path_follow->get_offset() + get_velocity() * delta);
//}

void OrbitPath2D::_draw() {draw_ellipse();}

void OrbitPath2D::draw_ellipse() {
	// REF: https://www.mathopenref.com/coordcirclealgorithm.html
	const int nb_points = get_draw_resolution();
	float theta = 0.0;  // angle that will be increased each loop
	const float step = M_2_PI / nb_points;  // amount to add to theta each time (radians)
    auto centroid = orbits::get_centroid_from_focus_point(eccentricity, semi_major_axis, argument_of_periapsis, get_focus_point());
	PoolVector2Array points;

	int i = 0;
	while (theta < M_2_PI) {
		const real_t x = semi_major_axis*Math::cos(theta) + centroid.x;
		const real_t y = get_semi_minor_axis()*Math::sin(theta) + centroid.y;
		// Rotate by argument of periapsis
		const real_t x_ = x*Math::cos(argument_of_periapsis) - y*godot::Math::sin(argument_of_periapsis);
		const real_t y_ = x*Math::sin(argument_of_periapsis) + y*godot::Math::cos(argument_of_periapsis);
		points.push_back(Vector2{x_, y_}-centroid);
		theta += step;
		i++;
	}
	for (int i = 0; i < nb_points-1; i++) {
		draw_line(points[i], points[i+1], draw_color);
	}
	draw_line(points[nb_points-1], points[0], draw_color);
}

// Important Functions
void OrbitPath2D::generate_path() {
    const float a = semi_major_axis;
    const float b = get_semi_minor_axis();
    auto curve = get_curve();
    auto centroid = orbits::get_centroid_from_focus_point(eccentricity, semi_major_axis, argument_of_periapsis, get_focus_point());
    curve->clear_points();
	curve->add_point(centroid+Vector2(b,0),godot::Vector2(0,-a));
	curve->add_point(centroid+Vector2(0,a),godot::Vector2(b,0));
	curve->add_point(centroid+Vector2(-b,0),godot::Vector2(0,a));
	curve->add_point(centroid+Vector2(0,-a),godot::Vector2(-b,0));
	curve->add_point(centroid+Vector2(b,0),godot::Vector2(0,-a));
}

// Setters
void OrbitPath2D::set_semi_major_axis(const float value) {
    semi_major_axis = value;
    _semi_minor_axis = orbits::get_semi_minor_axis(eccentricity, semi_major_axis);
    // _cached_angular_velocity = 0.0;
    generate_path();
    update();
}
void OrbitPath2D::set_eccentricity(const float value) {
    eccentricity = value;
    // _cached_angular_velocity = 0.0;
    generate_path();
    update();
}
void OrbitPath2D::set_argument_of_periapsis(const float value) {
    argument_of_periapsis = value;
    // _cached_angular_velocity = 0.0;
    generate_path();
    update();
}
//void OrbitPath2D::set_gravity(const float value) {
//    gravity = value;
//    _standard_gravitational_area = gravity / Math::pow(gravity_distance_scale, 2.0);
//    _cached_angular_velocity = 0.0;
//}
void OrbitPath2D::set_draw_resolution(const int value) {
    draw_resolution = value;
    generate_path();
    update();
}
void OrbitPath2D::set_draw_color(const Color value) {
    draw_color = value;
    generate_path();
    update();
}

// Getters
float OrbitPath2D::get_semi_major_axis() {return semi_major_axis;}
float OrbitPath2D::get_eccentricity() {return eccentricity;}
float OrbitPath2D::get_argument_of_periapsis() {return argument_of_periapsis;}
//float OrbitPath2D::get_gravity() {return gravity;}
int OrbitPath2D::get_draw_resolution() {return draw_resolution;}

// Memoized Getters
float OrbitPath2D::get_semi_minor_axis() {
  return _semi_minor_axis;
}
//Vector2 OrbitPath2D::get_velocity() {
//    // TODO: Handle circular obits with a cached velocity
//    Vector2 v = get_heliocentric_velocity_from_eccentric_anomaly(
//        get_eccentric_anomaly_from_position(position, focus_point),
//        eccentricity,
//        semi_major_axis,
//        argument_of_periapsis,
//        get_standard_gravitational_parameter()
//    );
//}
//PhysicsBody2D* OrbitPath2D::get_body() {
//    return _body;
//}
//PathFollow2D* OrbitPath2D::get_path_follow() {
//    return _path_follow;
//}
//Area2D* OrbitPath2D::get_area() {
//    return _area;
//}
//float OrbitPath2D::get_standard_gravitational_parameter() {
//    return _standard_gravitational_area;
//}
Vector2 OrbitPath2D::get_focus_point() {
    return get_position();
}
Color OrbitPath2D::get_draw_color() {
    return draw_color;
}
