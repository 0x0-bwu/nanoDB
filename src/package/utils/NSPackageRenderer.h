#pragma once
#include <nano/common>

namespace nano::package::utils {

class LayoutRenderer
{
public:
    explicit LayoutRenderer(CId<Layout> layout);
    bool WritePNG(std::string_view directory) const;

private:
    bool WritePNG(std::string_view filename, CId<StackupLayer> layer) const;
private:
    CId<Layout> m_layout;
    
};

} // namespace nano::package::utils