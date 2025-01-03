#pragma once
#include "NSPackage.h"

namespace nano::package {

class Padstack : public NamedObj, public Entity<Padstack>
{
public:
    struct Via
    {
    public:
        Float rotation{0};
        NCoord2D offset{0, 0};
        Id<Shape> shape;
    #ifdef NANO_BOOST_SERIALIZATION_SUPPORT
        template <typename Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            NS_UNUSED(version);
            ar & boost::serialization::make_nvp("rotation", rotation);
            ar & boost::serialization::make_nvp("offset", offset);
            ar & boost::serialization::make_nvp("shape", shape);
        }
    #endif//NANO_BOOST_SERIALIZATION_SUPPORT
    };

    struct Pad
    {
    public:
        Id<StackupLayer> layer;
        Float rotation{0};
        NCoord2D offset{0, 0};
        Id<Shape> shape;
    #ifdef NANO_BOOST_SERIALIZATION_SUPPORT
        template <typename Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            NS_UNUSED(version);
            ar & boost::serialization::make_nvp("layer", layer);
            ar & boost::serialization::make_nvp("rotation", rotation);
            ar & boost::serialization::make_nvp("offset", offset);
            ar & boost::serialization::make_nvp("shape", shape);
        }
    #endif//NANO_BOOST_SERIALIZATION_SUPPORT
    };

    struct Bump
    {
        Float thickness{0};
        Id<Shape> shape;
        Id<Material> material;
    #ifdef NANO_BOOST_SERIALIZATION_SUPPORT
        template <typename Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            NS_UNUSED(version);
            ar & boost::serialization::make_nvp("thickness", thickness);
            ar & boost::serialization::make_nvp("shape", shape);
            ar & boost::serialization::make_nvp("material", material);
        }
    #endif//NANO_BOOST_SERIALIZATION_SUPPORT
    };
    using Ball = Bump;
    Padstack(std::string name, Id<Package> package);
    Padstack() = default;

    void SetTopSolderBumpMaterial(Id<Material> material);
    void SetBotSolderBallMaterial(Id<Material> material);
    void SetTopSolderBumpParameters(Id<Shape> shape, Float thickness);
    void SetBotSolderBallParameters(Id<Shape> shape, Float thickness);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Package>, package),
    (Id<Material>, material),
    (Bump, solderBump),
    (Ball, solderBall),
    (Via, via),
    (std::vector<Pad>, pads))
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Padstack)
