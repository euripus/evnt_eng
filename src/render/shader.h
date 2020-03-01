#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>

namespace evnt
{
class IShader
{
public:
    IShader()          = default;
    virtual ~IShader() = default;

    uint32_t                 getID() const { return m_id; }
    std::string              getShaderInfoLog() const { return m_info_log; }
    std::vector<std::string> getShaderStageInfoLogs() const { return m_shader_info_logs; }
    bool                     isCompileSuccess() const { return m_compile_success; }

    virtual void destroy() = 0;

protected:
    bool                     m_compile_success = false;
    std::string              m_info_log;
    std::vector<std::string> m_shader_info_logs;
    uint32_t                 m_id = 0;
};
}   // namespace evnt

#endif   // SHADER_H
