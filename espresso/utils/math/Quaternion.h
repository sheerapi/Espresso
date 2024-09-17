#pragma once
#include "core/Logger.h"
#include "utils/math/Math.h"
#include "utils/math/Vector3.h"

namespace Espresso
{
	struct Quaternion
	{
	public:
		inline static constexpr int Components = 4;

		float W;
		float X;
		float Y;
		float Z;

		Quaternion() : W(1.0F), X(0.0F), Y(0.0F), Z(0.0F) {};

		Quaternion(float w, float x, float y, float z) : W(w), X(x), Y(y), Z(z) {};

		inline static auto FromAxisAngle(float angle, Vector3 aXis) -> Quaternion
		{
			float halfAngle = angle * 0.5F;
			float s = Math::Sin(halfAngle);
			return {Math::Cos(halfAngle), aXis[0] * s, aXis[1] * s, aXis[2] * s};
		}

		inline void Normalize()
		{
			float mag = Math::SquareRoot(W * W + X * X + Y * Y + Z * Z);
			W /= mag;
			X /= mag;
			Y /= mag;
			Z /= mag;
		}

		inline auto Normalized() -> Quaternion
		{
			float mag = Math::SquareRoot(W * W + X * X + Y * Y + Z * Z);
			return {W /= mag, X /= mag, Y /= mag, Z /= mag};
		}

		inline auto operator*(const Quaternion& q) const -> Quaternion
		{
			return {W * q.W - X * q.X - Y * q.Y - Z * q.Z,
					W * q.X + X * q.W + Y * q.Z - Z * q.Y,
					W * q.Y - X * q.Z + Y * q.W + Z * q.X,
					W * q.Z + X * q.Y - Y * q.X + Z * q.W};
		}

		[[nodiscard]] inline auto Rotate(Vector3 v) const -> Vector3
		{
			Quaternion qv(0, v[0], v[1], v[2]);
			Quaternion result = (*this) * qv * Inverse();
			return {result.X, result.Y, result.Z};
		}

		[[nodiscard]] inline auto Conjugate() const -> Quaternion
		{
			return {W, -X, -Y, -Z};
		}

		[[nodiscard]] inline auto Inverse() const -> Quaternion
		{
			float norm = W * W + X * X + Y * Y + Z * Z;
			return Conjugate() / norm;
		}

		inline auto operator/(float scalar) const -> Quaternion
		{
			return {W / scalar, X / scalar, Y / scalar, Z / scalar};
		}

		inline auto operator/(const Quaternion& val) const -> Quaternion
		{
			return {W / val.W, X / val.X, Y / val.Y, Z / val.Z};
		}

		inline auto operator*(float scalar) const -> Quaternion
		{
			return {W * scalar, X * scalar, Y * scalar, Z * scalar};
		}

		inline auto operator+(float scalar) const -> Quaternion
		{
			return {W + scalar, X + scalar, Y + scalar, Z + scalar};
		}

		inline auto operator+(const Quaternion& val) const -> Quaternion
		{
			return {W + val.W, X + val.X, Y + val.Y, Z + val.Z};
		}

		inline auto operator-(float scalar) const -> Quaternion
		{
			return {W - scalar, X - scalar, Y - scalar, Z - scalar};
		}

		inline auto operator-(const Quaternion& val) const -> Quaternion
		{
			return {W - val.W, X - val.X, Y - val.Y, Z - val.Z};
		}

		inline static auto Dot(const Quaternion& q1, const Quaternion& q2) -> float
		{
			return q1.W * q2.W + q1.X * q2.X + q1.Y * q2.Y + q1.Z * q2.Z;
		}

		[[nodiscard]] inline auto ToEuler() const -> Vector3
		{
			Vector3 angles;

			float sinr_cosp = 2 * (W * X + Y * Z);
			float cosr_cosp = 1 - 2 * (X * X + Y * Y);
			angles[0] = Math::Atan2(sinr_cosp, cosr_cosp);

			float sinp = 2 * (W * Y - Z * X);
			if (Math::Abs(sinp) >= 1)
			{
				angles[1] = (float)std::copysign(Math::HalfPi, sinp);
			}
			else
			{
				angles[1] = Math::Asin(sinp);
			}

			float siny_cosp = 2 * (W * Z + X * Y);
			float cosy_cosp = 1 - 2 * (Y * Y + Z * Z);
			angles[2] = Math::Atan2(siny_cosp, cosy_cosp);

			return angles;
		}

		inline static auto Slerp(const Quaternion& start, const Quaternion& end,
								 float t) -> Quaternion
		{
			float dot = Dot(start, end);

			// Clamp dot product to avoid numerical errors
			dot = Math::Max(-1.0F, Math::Min(1.0F, dot));

			if (dot > 0.9995F)
			{
				// Linear interpolation if quaternions are very close
				return Quaternion(start.W + t * (end.W - start.W),
								  start.X + t * (end.X - start.X),
								  start.Y + t * (end.Y - start.Y),
								  start.Z + t * (end.Z - start.Z))
					.Normalized();
			}

			float theta_0 = Math::Acos(dot);
			float theta = theta_0 * t;

			Quaternion q3 = end - start * dot;
			q3.Normalize();

			return start * Math::Cos(theta) + q3 * Math::Sin(theta);
		}

		static auto RotateTowards(const Quaternion& from, const Quaternion& to,
								  float maxDegreesDelta) -> Quaternion
		{
			float maxRadiansDelta = Math::DegreesToRadians(maxDegreesDelta);
			float angle = Angle(from, to);
			if (angle == 0.0F)
			{
				return to;
			}

			float t = std::fmin(1.0F, maxRadiansDelta / angle);
			return Slerp(from, to, t);
		}

		inline static auto Angle(const Quaternion& q1, const Quaternion& q2) -> float
		{
			float dot = Dot(q1, q2);
			return Math::Acos(2 * dot * dot - 1);
		}

		inline static auto LookRotation(const Vector3& forward,
										const Vector3& up = {0.0F, 1.0F,
															 0.0F}) -> Quaternion
		{
			auto f = forward.Normalized();
			auto r = Vector3::Cross(up, f).Normalized();
			auto u = Vector3::Cross(f, r);

			float m00 = r[0];
			float m01 = r[1];
			float m02 = r[2];
			float m10 = u[0];
			float m11 = u[1];
			float m12 = u[2];
			float m20 = f[0];
			float m21 = f[1];
			float m22 = f[2];

			float t = m00 + m11 + m22;

			Quaternion q;
			if (t > 0.0F)
			{
				float s = Math::SquareRoot(t + 1.0F) * 2.0F;
				q.W = 0.25F * s;
				q.X = (m21 - m12) / s;
				q.Y = (m02 - m20) / s;
				q.Z = (m10 - m01) / s;
			}
			else if ((static_cast<int>(m00 > m11) & static_cast<int>(m00 > m22)) != 0)
			{
				float s = Math::SquareRoot(1.0F + m00 - m11 - m22) * 2.0F;
				q.W = (m21 - m12) / s;
				q.X = 0.25F * s;
				q.Y = (m01 + m10) / s;
				q.Z = (m02 + m20) / s;
			}
			else if (m11 > m22)
			{
				float s = Math::SquareRoot(1.0F + m11 - m00 - m22) * 2.0F;
				q.W = (m02 - m20) / s;
				q.X = (m01 + m10) / s;
				q.Y = 0.25F * s;
				q.Z = (m12 + m21) / s;
			}
			else
			{
				float s = Math::SquareRoot(1.0F + m22 - m00 - m11) * 2.0F;
				q.W = (m10 - m01) / s;
				q.X = (m02 + m20) / s;
				q.Y = (m12 + m21) / s;
				q.Z = 0.25F * s;
			}

			return q;
		}

		auto operator[](int index) -> float&
		{
			es_coreAssert(index < Components && index >= 0, "Out of bounds");

			switch (index)
			{
			case 0:
				return W;

			case 1:
				return X;

			case 2:
				return Y;

			case 3:
				return Z;

			default:
				return W;
			}
		}

		auto operator*(Vector3 q) const -> Vector3
		{
			Quaternion v_q(0, q.X, q.Y, q.Z);

			Quaternion q_conjugate = Conjugate();
			Quaternion rotated_q = *this * v_q * q_conjugate;

			return {rotated_q.X, rotated_q.Y, rotated_q.Z};
		}

		static const Quaternion Zero;
	};

	inline const Quaternion Quaternion::Zero(0.0F, 0.0F, 0.0F, 0.0F);
}