#include <molecli/molecli.hpp>

#include <string>
#include <sstream>

struct Vector3D {
    float x;
    float y;
    float z;

    Vector3D operator+(const Vector3D &other) {
        return Vector3D{this->x + other.x, this->y + other.y, this->z + other.z};
    }

    Vector3D operator-(const Vector3D &other) {
        return Vector3D{this->x - other.x, this->y - other.y, this->z - other.z};
    }

    float operator*(const Vector3D &other) {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    friend std::ostream &operator<<(std::ostream &stream, const Vector3D &obj) {
        stream << obj.x << " " << obj.y << " " << obj.z;

        return stream;
    }
};

std::optional<Vector3D> vector3d_from_string(std::string &&token) {
    std::stringstream ss(token);
    std::vector<std::string> result;

    while(ss.good()) {
        std::string substr;
        std::getline(ss, substr, ',');
        result.push_back(substr);
    }

    if (result.size() != 3) {
        return std::nullopt;
    }
    else {
        try {
            Vector3D vec;
            vec.x = std::stof(result[0]);
            vec.y = std::stof(result[1]);
            vec.z = std::stof(result[2]);
            return vec;
        }
        catch (...) {
            return std::nullopt;
        }
    }
}

MOLECLI_ADD_CASTER(Vector3D, vector3d_from_string);

void echo_vector(Vector3D vec) {
    std::cout << vec << '\n';
}

void add_vectors(Vector3D a, Vector3D b) {
    std::cout << a + b << '\n';
}

void subtract_vectors(Vector3D a, Vector3D b) {
    std::cout << a - b << '\n';
}

void multiply_vectors(Vector3D a, Vector3D b) {
    std::cout << a * b << '\n';
}

int main() {
    molecli::CLI cli;

    cli.add_command("echo", "Echoes a given vector", echo_vector);
    cli.add_command("add", "Adds two vectors", add_vectors);
    cli.add_command("subtract", "Subtracts two vectors", subtract_vectors);
    cli.add_command("multiply", "Performs scalar multiplication of two vectors", multiply_vectors);

    cli.run_loop();
}