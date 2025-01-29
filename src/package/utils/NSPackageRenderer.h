#pragma once
#include <nano/common>

namespace nano::package::utils {

class LayoutRenderer
{
public:
    explicit LayoutRenderer(CId<Layout> layout, size_t width = 4096);
    bool WritePNG(std::string_view directory) const;

private:
    bool WritePNG(std::string_view filename, CId<StackupLayer> layer) const;
private:
    CId<Layout> m_layout;
    size_t m_width;
};

} // namespace nano::package::utils