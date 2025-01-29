#pragma once
#include "TestCommon.hpp"
#include <nano/core/package>

using namespace boost::unit_test;
using namespace nano::package::extension;

void t_kicad1()
{
    Database::Create("kicad_test1");

    auto filename = generic::fs::DirName(__FILE__).string() + "/data/package/test.kicad_pcb";
    auto pkg = CreateFromKiCad(filename.c_str());
    auto top = pkg->GetTop();
    top->Flatten();

    auto layout = top->GetFlattenedLayout();

    nano::package::utils::LayoutRenderer renderer(layout);
    renderer.WritePNG(generic::fs::DirName(filename).string() + "/test");
    BOOST_CHECK(pkg);
    Database::Shutdown();
}

void t_kicad2()
{
    Database::Create("kicad_test2");

    auto filename = generic::fs::DirName(__FILE__).string() + "/data/package/jetson-nano-baseboard.kicad_pcb";
    auto pkg = CreateFromKiCad(filename.c_str());

    auto top = pkg->GetTop();

    auto layout = top->GetCell()->GetLayout();
    nano::package::utils::LayoutRenderer renderer(layout);
    renderer.WritePNG(generic::fs::DirName(filename).string() + "/jetson-nano-baseboard");
    BOOST_CHECK(pkg);
    Database::Shutdown();
}

test_suite * create_nano_package_extension_test_suite()
{
    test_suite * package_extension_suite = BOOST_TEST_SUITE("s_package_extension");
    //
    package_extension_suite->add(BOOST_TEST_CASE(&t_kicad1));
    package_extension_suite->add(BOOST_TEST_CASE(&t_kicad2));
    //
    return package_extension_suite;
}