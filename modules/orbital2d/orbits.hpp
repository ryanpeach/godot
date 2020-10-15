#include <Godot.hpp>

#pragma once

namespace orbits {

    struct PositionVelocity2D {
        godot::Vector2 p;
        godot::Vector2 v;
    };

    float get_semi_minor_axis(
            const float eccentricity,
            const float semi_major_axis
    );

    float get_linear_eccentricity(
            const float eccentricity,
            const float semi_major_axis
    );

    float get_geocentric_distance(
            const float standard_gravitational_parameter,
            const float rotational_period
    );

    godot::Vector2 get_focus_point_from_centroid(
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const godot::Vector2 centroid
    );

    godot::Vector2 get_centroid_from_focus_point(
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const godot::Vector2 focus
    );

    float get_orbital_period(
            const float semi_major_axis,
            const float standard_gravitational_parameter
    );

    float get_mean_angular_motion(
            const float semi_major_axis,
            const float standard_gravitational_parameter
    );

    float get_mean_anomaly(
            const float time,
            const float semi_major_axis,
            const float standard_gravitational_parameter
    );

    float get_eccentric_anomaly_from_mean_anomaly(
            const float mean_anomaly,
            const float eccentricity,
            const float argument_of_periapsis
    );

    float get_eccentric_anomaly(
            const float time,
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const float standard_gravitational_parameter
    );

    float get_eccentric_anomaly_from_position(
            const godot::Vector2 position,
            const godot::Vector2 focus_point
    );

    float get_true_anomaly_from_eccentric_anomaly(
            const float eccentric_anomaly,
            const float eccentricity
    );

    float get_true_anomaly_from_time(
            const float time,
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const float standard_gravitational_parameter
    );

    float get_heliocentric_distance_from_eccentric_anomaly(
            const float eccentric_anomaly,
            const float eccentricity,
            const float semi_major_axis
    );

    float get_heliocentric_distance_from_time(
            const float time,
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const float standard_gravitational_parameter
    );

    godot::Vector2 get_heliocentric_velocity_from_eccentric_anomaly(
            const float eccentric_anomaly,
            const float eccentricity,
            const float semi_major_axis,
            const float standard_gravitational_parameter
    );

    godot::Vector2 get_heliocentric_velocity_from_time(
            const float time,
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const float standard_gravitational_parameter
    );

    PositionVelocity2D get_heliocentric_position_velocity_from_eccentric_anomaly(
            const float eccentric_anomaly,
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const float standard_gravitational_parameter
    );

    PositionVelocity2D get_heliocentric_position_velocity_from_time(
            const float time,
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const float standard_gravitational_parameter
    );

    bool is_circle(float eccentricity);

    bool is_ellipse(float eccentricity);

    bool is_parabola(float eccentricity);

    bool is_hyperbola(float eccentricity);

}

// TODO
// float time_to_true_anomaly(const float true_anomaly);

// TODO
// Vector<Vector2> find_intercept_points(
//     const float eccentricity1, const float semi_major_axis1, const float argument_of_periapsis1,
//     const float eccentricity2, const float semi_major_axis2, const float argument_of_periapsis2
// );
