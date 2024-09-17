#pragma once
#include "utils/math/Quaternion.h"
#include "utils/math/Vector4.h"
#include <array>

namespace Espresso
{
	struct Matrix4
	{
	public:
		inline static constexpr int Components = 4;
		Vector4 Row0;
		Vector4 Row1;
		Vector4 Row2;
		Vector4 Row3;

		Matrix4()
		{
			Row0 = Vector4(1, 0, 0, 0);
			Row1 = Vector4(0, 1, 0, 0);
			Row2 = Vector4(0, 0, 1, 0);
			Row3 = Vector4(0, 0, 0, 1);
		}

		Matrix4(float m00, float m01, float m02, float m03, float m10, float m11,
				float m12, float m13, float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
		{
			Row0 = Vector4(m00, m01, m02, m03);
			Row1 = Vector4(m10, m11, m12, m13);
			Row2 = Vector4(m20, m21, m22, m23);
			Row3 = Vector4(m30, m31, m32, m33);
		}

		inline auto operator()(int row, int col) -> float&
		{
			return (*this)[row][col];
		}

		inline auto operator*(Matrix4 other) -> Matrix4
		{
			Matrix4 result;
			for (int row = 0; row < 4; ++row)
			{
				for (int col = 0; col < 4; ++col)
				{
					result(row, col) = (*this)[row].Dot(Vector4(
						other(0, col), other(1, col), other(2, col), other(3, col)));
				}
			}
			return result;
		}

		inline auto operator*(const Vector4& vec) const -> Vector4
		{
			return {Row0.Dot(vec), Row1.Dot(vec), Row2.Dot(vec), Row3.Dot(vec)};
		}

		auto operator+(Matrix4& other) -> Matrix4
		{
			Matrix4 result;
			for (int i = 0; i < 4; ++i)
			{
				result[i] = (*this)[i] + other[i];
			}
			return result;
		}

		auto operator-(Matrix4& other) -> Matrix4
		{
			Matrix4 result;
			for (int i = 0; i < 4; ++i)
			{
				result[i] = (*this)[i] - other[i];
			}
			return result;
		}

		auto operator*(float scalar) -> Matrix4
		{
			Matrix4 result;
			for (int i = 0; i < 4; ++i)
			{
				result[i] = (*this)[i] * scalar;
			}
			return result;
		}

		auto Transpose() -> Matrix4
		{
			Matrix4 result;
			for (int row = 0; row < 4; ++row)
			{
				for (int col = 0; col < 4; ++col)
				{
					result(row, col) = (*this)(col, row); // NOLINT
				}
			}
			return result;
		}

		static auto LookAt(const Vector4& eye, const Vector4& center,
						   const Vector4& up) -> Matrix4
		{
			Vector4 f = (center - eye).Normalized();
			Vector4 u = up.Normalized();
			Vector4 s = Vector4::Cross(f, u).Normalized();
			u = Vector4::Cross(s, f);

			Matrix4 result;
			result(0, 0) = s.X;
			result(0, 1) = s.Y;
			result(0, 2) = s.Z;
			result(0, 3) = -s.Dot(eye);
			result(1, 0) = u.X;
			result(1, 1) = u.Y;
			result(1, 2) = u.Z;
			result(1, 3) = -u.Dot(eye);
			result(2, 0) = -f.X;
			result(2, 1) = -f.Y;
			result(2, 2) = -f.Z;
			result(2, 3) = f.Dot(eye);
			return result;
		}

		static auto Perspective(float fov, float aspectRatio, float near,
								float far) -> Matrix4
		{
			float tanHalfFov = Math::Tan(Math::DegreesToRadians(fov) / 2.0F);
			Matrix4 result;
			result(0, 0) = 1.0F / (aspectRatio * tanHalfFov);
			result(1, 1) = 1.0F / tanHalfFov;
			result(2, 2) = -(far + near) / (far - near);
			result(2, 3) = -(2.0F * far * near) / (far - near);
			result(3, 2) = -1.0F;
			result(3, 3) = 0.0F;
			return result;
		}

		static auto Ortho(float left, float right, float bottom, float top, float near,
						  float far) -> Matrix4
		{
			Matrix4 result;
			result(0, 0) = 2.0F / (right - left);
			result(1, 1) = 2.0F / (top - bottom);
			result(2, 2) = -2.0F / (far - near);
			result(0, 3) = -(right + left) / (right - left);
			result(1, 3) = -(top + bottom) / (top - bottom);
			result(2, 3) = -(far + near) / (far - near);
			return result;
		}

		static auto Frustum(float left, float right, float bottom, float top, float near,
							float far) -> Matrix4
		{
			Matrix4 result;
			result(0, 0) = (2.0F * near) / (right - left);
			result(1, 1) = (2.0F * near) / (top - bottom);
			result(0, 2) = (right + left) / (right - left);
			result(1, 2) = (top + bottom) / (top - bottom);
			result(2, 2) = -(far + near) / (far - near);
			result(2, 3) = -(2.0F * far * near) / (far - near);
			result(3, 2) = -1.0F;
			return result;
		}

		static auto Translate(const Vector4& translation) -> Matrix4
		{
			Matrix4 result;
			result(0, 3) = translation.X;
			result(1, 3) = translation.Y;
			result(2, 3) = translation.Z;
			return result;
		}

		static auto Rotate(const Vector4& axis, float angle) -> Matrix4
		{
			angle = Math::DegreesToRadians(angle);
			Vector4 normalizedAxis = axis.Normalized();
			float cosTheta = cosf(angle);
			float sinTheta = sinf(angle);
			float oneMinusCosTheta = 1.0F - cosTheta;

			Matrix4 result;
			result(0, 0) =
				cosTheta + normalizedAxis.X * normalizedAxis.Z * oneMinusCosTheta;
			result(0, 1) = normalizedAxis.X * normalizedAxis.Y * oneMinusCosTheta -
						   normalizedAxis.Z * sinTheta;
			result(0, 2) = normalizedAxis.X * normalizedAxis.Z * oneMinusCosTheta +
						   normalizedAxis.Y * sinTheta;

			result(1, 0) = normalizedAxis.Y * normalizedAxis.X * oneMinusCosTheta +
						   normalizedAxis.Z * sinTheta;
			result(1, 1) =
				cosTheta + normalizedAxis.Y * normalizedAxis.Y * oneMinusCosTheta;
			result(1, 2) = normalizedAxis.Y * normalizedAxis.Z * oneMinusCosTheta -
						   normalizedAxis.X * sinTheta;

			result(2, 0) = normalizedAxis.Z * normalizedAxis.X * oneMinusCosTheta -
						   normalizedAxis.Y * sinTheta;
			result(2, 1) = normalizedAxis.Z * normalizedAxis.Y * oneMinusCosTheta +
						   normalizedAxis.X * sinTheta;
			result(2, 2) =
				cosTheta + normalizedAxis.Z * normalizedAxis.Z * oneMinusCosTheta;

			return result;
		}

		static auto Scale(const Vector4& scale) -> Matrix4
		{
			Matrix4 result;
			result(0, 0) = scale.X;
			result(1, 0) = scale.Y;
			result(2, 0) = scale.Z;
			return result;
		}

		inline auto ToPrimitive() -> std::array<float, 16>
		{
			return {Row0.X, Row0.Y, Row0.Z, Row0.W, Row1.X, Row1.Y, Row1.Z, Row1.W,
					Row2.X, Row2.Y, Row2.Z, Row2.W, Row3.X, Row3.Y, Row3.Z, Row3.W};
		}

		inline static auto ToMatrix(const Quaternion& quat) -> Matrix4
		{
			Matrix4 mat;

			mat[0][0] = 1 - 2 * (quat.Y * quat.Y + quat.X * quat.Z);
			mat[0][1] = 2 * (quat.X * quat.Y - quat.Z * quat.W);
			mat[0][2] = 2 * (quat.X * quat.Z + quat.Y * quat.W);
			mat[0][3] = 0.0F;

			mat[1][0] = 2 * (quat.X * quat.Y + quat.Z * quat.W);
			mat[1][1] = 1 - 2 * (quat.X * quat.X + quat.Z * quat.Z);
			mat[1][2] = 2 * (quat.Y * quat.Z - quat.X * quat.W);
			mat[1][3] = 0.0F;

			mat[2][0] = 2 * (quat.X * quat.Z - quat.Y * quat.W);
			mat[2][1] = 2 * (quat.Y * quat.Z + quat.X * quat.W);
			mat[2][2] = 1 - 2 * (quat.X * quat.X + quat.Y * quat.Y);
			mat[2][3] = 0.0F;

			mat[3][0] = 0.0F;
			mat[3][1] = 0.0F;
			mat[3][2] = 0.0F;
			mat[3][3] = 1.0F;

			return mat;
		}

		auto operator[](int index) -> Vector4&
		{
			es_coreAssert(index < Components && index >= 0, "Out of bounds");

			switch (index)
			{
			case 0:
				return Row0;

			case 1:
				return Row1;

			case 2:
				return Row2;

			case 3:
				return Row3;

			default:
				return Row0;
			}
		}
	};
}