from tiny_haversine import haversine


def test_haversine_sf_to_la_is_reasonable():
    # San Francisco to Los Angeles is about 559 km.
    km = haversine(37.7749, -122.4194, 34.0522, -118.2437)
    assert 540 <= km <= 580
