#include "kepler.hpp"
#include "orbits.hpp"

namespace orbits {

    float get_semi_minor_axis(const float eccentricity, const float semi_major_axis) {
        return godot::Math::sqrt(
                pow(semi_major_axis, 2.0) - pow(get_linear_eccentricity(eccentricity, semi_major_axis), 2.0));
    }

    float get_linear_eccentricity(const float eccentricity, const float semi_major_axis) {
        return eccentricity * semi_major_axis;
    }

    float get_geocentric_distance(
            const float standard_gravitational_parameter,
            const float rotational_period
    ) {
        return pow(standard_gravitational_parameter * pow(rotational_period, 2.0) / (4.0 * pow(M_PI, 2.0)), 1.0 / 3.0);
    }

    godot::Vector2 get_focus_point_from_centroid(
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const godot::Vector2 centroid
    ) {
        float le = get_linear_eccentricity(eccentricity, semi_major_axis);
        return godot::Vector2{
                le * godot::Math::sin(argument_of_periapsis) + centroid.x,
                le * godot::Math::cos(argument_of_periapsis) + centroid.y
        };
    }

    godot::Vector2 get_centroid_from_focus_point(
            const float eccentricity,
            const float semi_major_axis,
            const float argument_of_periapsis,
            const godot::Vector2 focus
    ) {
        float le = get_linear_eccentricity(eccentricity, semi_major_axis);
        return godot::Vector2{
            focus.x - le * godot::Math::sin(argument_of_periapsis),
            focus.y - le * godot::Math::cos(argument_of_periapsis)
        };
    }

    float get_orbital_period(const float semi_major_axis, const float standard_gravitational_parameter) {
        return 2.0 * M_PI * (godot::Math::sqrt(pow(semi_major_axis, 3.0) / standard_gravitational_parameter));
    }

    float get_mean_angular_motion(const float semi_major_axis, const float standard_gravitational_parameter) {
        return 2.0 * M_PI / get_orbital_period(semi_major_axis, standard_gravitational_parameter);
    }

    float
    get_mean_anomaly(const float time, const float semi_major_axis, const float standard_gravitational_parameter) {
        return get_mean_angular_motion(semi_major_axis, standard_gravitational_parameter) * time;
    }

    float get_eccentric_anomaly_from_mean_anomaly(const float mean_anomaly, const float eccentricity,
                                                  const float argument_of_periapsis) {
        return kepler::ecc_anomaly(eccentricity, mean_anomaly) + argument_of_periapsis;
    }

    float get_eccentric_anomaly_from_position(
            const godot::Vector2 position,
            const godot::Vector2 focus_point
    ) {
        return focus_point.angle_to(position);
    }

    float get_eccentric_anomaly(const float time, const float eccentricity, const float semi_major_axis,
                                const float argument_of_periapsis,
                                const float standard_gravitational_parameter) {
        float mean_anomaly = get_mean_anomaly(time, semi_major_axis, standard_gravitational_parameter);
        return get_eccentric_anomaly_from_mean_anomaly(mean_anomaly, eccentricity, argument_of_periapsis);
    }

    float get_true_anomaly_from_eccentric_anomaly(const float eccentric_anomaly, const float eccentricity) {
        const float E = eccentric_anomaly;
        return 2.0 * godot::Math::atan2(godot::Math::sqrt(1.0 + eccentricity) * godot::Math::sin(E / 2.0),
                                        godot::Math::sqrt(1.0 - eccentricity) * godot::Math::cos(E / 2.0));
    }

    float get_true_anomaly_from_time(const float time, const float eccentricity, const float semi_major_axis,
                                     const float argument_of_periapsis,
                                     const float standard_gravitational_parameter) {
        float E = get_eccentric_anomaly(time, eccentricity, semi_major_axis, argument_of_periapsis,
                                        standard_gravitational_parameter);
        return get_true_anomaly_from_eccentric_anomaly(E, eccentricity);
    }

    float
    get_heliocentric_distance_from_eccentric_anomaly(const float eccentric_anomaly, const float eccentricity,
                                                     const float semi_major_axis) {
        return semi_major_axis * (1.0 - eccentricity * godot::Math::cos(eccentric_anomaly));
    }

    float
    get_heliocentric_distance_from_time(const float time, const float eccentricity, const float semi_major_axis,
                                        const float argument_of_periapsis,
                                        const float standard_gravitational_parameter) {
        float eccentric_anomaly = get_eccentric_anomaly(time, eccentricity, semi_major_axis, argument_of_periapsis,
                                                        standard_gravitational_parameter);
        return get_heliocentric_distance_from_eccentric_anomaly(eccentric_anomaly, eccentricity, semi_major_axis);
    }

    godot::Vector2
    get_heliocentric_velocity_from_eccentric_anomaly(const float eccentric_anomaly, const float eccentricity,
                                                     const float semi_major_axis,
                                                     const float standard_gravitational_parameter) {
        float heliocentric_distance = get_heliocentric_distance_from_eccentric_anomaly(eccentric_anomaly, eccentricity,
                                                                                       semi_major_axis);
        float scale = godot::Math::sqrt(standard_gravitational_parameter * semi_major_axis) / heliocentric_distance;
        return godot::Vector2(
                scale * -godot::Math::sin(eccentric_anomaly),
                scale * godot::Math::sqrt(1.0 - pow(eccentricity, 2.0)) * godot::Math::cos(eccentric_anomaly)
        );
    }

    godot::Vector2
    get_heliocentric_velocity_from_time(const float time, const float eccentricity, const float semi_major_axis,
                                        const float argument_of_periapsis,
                                        const float standard_gravitational_parameter) {
        float eccentric_anomaly = get_eccentric_anomaly(time, eccentricity, semi_major_axis, argument_of_periapsis,
                                                        standard_gravitational_parameter);
        return get_heliocentric_velocity_from_eccentric_anomaly(eccentric_anomaly, eccentricity, semi_major_axis,
                                                                standard_gravitational_parameter);
    }

    PositionVelocity2D
    get_heliocentric_position_velocity_from_eccentric_anomaly(const float eccentric_anomaly, const float eccentricity,
                                                              const float semi_major_axis,
                                                              const float standard_gravitational_parameter) {
        float heliocentric_distance = get_heliocentric_distance_from_eccentric_anomaly(eccentric_anomaly, eccentricity,
                                                                                       semi_major_axis);
        float scale = godot::Math::sqrt(standard_gravitational_parameter * semi_major_axis) / heliocentric_distance;
        godot::Vector2 v = godot::Vector2(
                scale * -godot::Math::sin(eccentric_anomaly),
                scale * godot::Math::sqrt(1.0 - pow(eccentricity, 2.0)) * godot::Math::cos(eccentric_anomaly)
        );
        float distance = semi_major_axis * (1.0 - eccentricity * godot::Math::cos(eccentric_anomaly));
        float true_anomaly = get_true_anomaly_from_eccentric_anomaly(eccentric_anomaly, eccentricity);
        godot::Vector2 p = godot::Vector2(
                distance * godot::Math::cos(true_anomaly),
                distance * godot::Math::sin(true_anomaly)
        );
        return PositionVelocity2D{p, v};
    }

    PositionVelocity2D get_heliocentric_position_velocity_from_time(const float time, const float eccentricity,
                                                                    const float semi_major_axis,
                                                                    const float argument_of_periapsis,
                                                                    const float standard_gravitational_parameter) {
        float eccentric_anomaly = get_eccentric_anomaly(time, eccentricity, semi_major_axis, argument_of_periapsis,
                                                        standard_gravitational_parameter);
        return get_heliocentric_position_velocity_from_eccentric_anomaly(eccentric_anomaly, eccentricity,
                                                                         semi_major_axis,
                                                                         standard_gravitational_parameter);
    }

    bool is_circle(float eccentricity) {
        return eccentricity == 0.0;
    }

    bool is_ellipse(float eccentricity) {
        return eccentricity > 0.0 && eccentricity < 1.0;
    }

    bool is_parabola(float eccentricity) {
        return eccentricity == 1.0;
    }

    bool is_hyperbola(float eccentricity) {
        return eccentricity > 1.0;
    }

}