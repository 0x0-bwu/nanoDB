#pragma once
#include <nano/common>
namespace nano::package
{

    class Padstack : public NamedObj, public Entity<Padstack>
    {
    public:
        struct Via
        {
            BOOST_HANA_DEFINE_STRUCT(Via,
            (Float, rotation),
            (NCoord2D, offset),
            (Id<Shape>, shape));
            Via() { NS_INIT_HANA_STRUCT(*this) }
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
            template <typename Archive>
            void serialize(Archive &ar, const unsigned int version)
            {
                NS_UNUSED(version);
                NS_SERIALIZATION_HANA_STRUCT(ar, *this);
            }
#endif // NANO_BOOST_SERIALIZATION_SUPPORT
        };

        struct Pad
        {
            BOOST_HANA_DEFINE_STRUCT(Pad,
            (Id<StackupLayer>, layer),
            (Float, rotation),
            (NCoord2D, offset),
            (Id<Shape>, shape));
            Pad() { NS_INIT_HANA_STRUCT(*this) }
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
            template <typename Archive>
            void serialize(Archive &ar, const unsigned int version)
            {
                NS_UNUSED(version);
                NS_SERIALIZATION_HANA_STRUCT(ar, *this);
            }
#endif // NANO_BOOST_SERIALIZATION_SUPPORT
        };

        struct Bump
        {
            BOOST_HANA_DEFINE_STRUCT(Bump,
            (Float, thickness),
            (Id<Shape>, shape),
            (Id<Material>, material));
            Bump() { NS_INIT_HANA_STRUCT(*this) }
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
            template <typename Archive>
            void serialize(Archive &ar, const unsigned int version)
            {
                NS_UNUSED(version);
                NS_SERIALIZATION_HANA_STRUCT(ar, *this);
            }
#endif // NANO_BOOST_SERIALIZATION_SUPPORT
        };
        using Ball = Bump;
        Padstack(std::string name, Id<Package> package);

        void SetTopSolderBumpMaterial(Id<Material> material);
        void SetBotSolderBallMaterial(Id<Material> material);
        void SetTopSolderBumpParameters(Id<Shape> shape, Float thickness);
        void SetBotSolderBallParameters(Id<Shape> shape, Float thickness);

    private:
        Padstack();
        NS_SERIALIZATION_FUNCTIONS_DECLARATION
        NS_CLASS_MEMBERS_DEFINE(
            (Id<Package>, package),
            (Id<Material>, material),
            (Bump, solderBump),
            (Ball, solderBall),
            (Via, via),
            (std::vector<Pad>, pads))
    };

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Padstack)
