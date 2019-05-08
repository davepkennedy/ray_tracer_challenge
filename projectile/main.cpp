#include <tuple.h>
#include <canvas.h>
#include <iostream>
#include <fstream>

class environment {
public:
	const rt::tuple<float,4> gravity;
	const rt::tuple<float,4> wind;
public:
	environment(const rt::tuple<float,4>& g, const rt::tuple<float,4>& w)
		: gravity(g)
		, wind(w)
	{}
};

class projectile {
public:
	rt::tuple<float,4> position;
	rt::tuple<float,4> velocity;
public:
	projectile(const rt::tuple<float,4>& p, const rt::tuple<float,4>& v)
		: position(p)
		, velocity(v)
	{}
};

projectile tick(const environment& env, const projectile& proj)
{
	auto pos = proj.position + proj.velocity;
	auto vel = proj.velocity + env.gravity + env.wind;
	return projectile(pos, vel);
}

std::ostream& operator<<(std::ostream& s, const rt::tuple<float,4>& t) {
	s << "(" << t[0] << "," << t[1] << "," << t[2] << ")";
	return s;
}

std::ostream& operator<<(std::ostream& s, const projectile& p) {
	s << "projectile(" << p.position << ", " << p.velocity << ")";
	return s;
}

int main(int argc, char* argv[]) {
	auto start = rt::point<float>(0, 1, 0);
	auto velocity = rt::vector(1.f, 1.8f, 0.f).normalize() * 11.25f;
	projectile p = projectile(start, velocity);

	auto gravity = rt::vector(0.f, -0.1f, 0.f);
	auto wind = rt::vector(-0.01f, 0.f, 0.f);
	environment e = environment(gravity, wind);

	auto canvas = rt::canvas(900, 550);
	auto color = rt::color(1, 0, 0);

	while (p.position[1] > 0) {
		canvas[{(int)p.position[0], 550-(int)p.position[1]}] = color;
		p = tick(e, p);
	}

	auto ppm = canvas.toPpm();

	std::ofstream file;
	file.open("projection.ppm");
	file << ppm;
	file.close();
}