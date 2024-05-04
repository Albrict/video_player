#pragma once
#include "SDL_stdinc.h"
#include <vector>

namespace VP {
        struct YuvData final {
            std::vector<Uint8> m_y_plane;
            std::vector<Uint8> m_u_plane;
            std::vector<Uint8> m_v_plane;
            int                m_uv_pitch;

            explicit YuvData(const size_t y_plane_size, const size_t uv_plane_size, const int uv_pitch)
                : m_y_plane(y_plane_size),
                m_u_plane(uv_plane_size),
                m_v_plane(uv_plane_size),
                m_uv_pitch(uv_pitch) {}
            YuvData(const YuvData &other)
            {
                m_y_plane  = other.m_y_plane; 
                m_u_plane  = other.m_u_plane;
                m_v_plane  = other.m_v_plane;
                m_uv_pitch = other.m_uv_pitch;
            }

            YuvData(YuvData &&other) noexcept
            {
                m_y_plane  = std::move(other.m_y_plane);
                m_u_plane  = std::move(other.m_u_plane);
                m_v_plane  = std::move(other.m_v_plane);
                m_uv_pitch = other.m_uv_pitch;
            }
            
            YuvData &operator=(const YuvData &rhs)
            { return *this = YuvData(rhs); }

            YuvData &operator=(YuvData &&rhs) noexcept
            { return *this = YuvData(std::move(rhs)); }
            
            friend constexpr bool operator==(const YuvData &lhs, const YuvData &&rhs)
            {
                return ( lhs.m_uv_pitch == rhs.m_uv_pitch
                        && lhs.m_y_plane == rhs.m_y_plane 
                        && lhs.m_u_plane == rhs.m_u_plane
                        && lhs.m_v_plane == rhs.m_v_plane);
            }
            ~YuvData() = default;
        };
}
