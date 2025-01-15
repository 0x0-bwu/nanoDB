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
            (CId<Shape>, shape));
            Via() { NS_INIT_HANA_STRUCT(*this) }
            UPtr<Shape> GetShape() const;
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
            (Float, rotation),
            (NCoord2D, offset),
            (CId<Shape>, shape));
            Pad() { NS_INIT_HANA_STRUCT(*this) }
            UPtr<Shape> GetShape() const;
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
            (CId<Shape>, shape),
            (CId<Material>, material));
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
        Padstack(std::string name, CId<Package> package);

        void SetTopSolderBumpMaterial(CId<Material> material);
        void SetBotSolderBallMaterial(CId<Material> material);
        CId<Material> GetTopSolderBumpMaterial() const;
        CId<Material> GetBotSolderBallMaterial() const;
        
        void SetTopSolderBumpParameters(CId<Shape> shape, Float thickness);
        void SetBotSolderBallParameters(CId<Shape> shape, Float thickness);
        bool GetTopSolderBumpParameters(CId<Shape> & shape, Float & thickness) const;
        bool GetBotSolderBallParameters(CId<Shape> & shape, Float & thickness) const;

        bool hasTopSolderBump() const;
        bool hasBotSolderBall() const;

        CId<Material> GetMaterial() const;

        UPtr<Shape> GetPadShape(CId<StackupLayer> layer) const;
        UPtr<Shape> GetViaShape() const;

    private:
        Padstack();
        using Pads = HashMap<CId<StackupLayer>, Pad>;
        NS_SERIALIZATION_FUNCTIONS_DECLARATION
        NS_CLASS_MEMBERS_DEFINE(
            (CId<Package>, package),
            (CId<Material>, material),
            (Bump, solderBump),
            (Ball, solderBall),
            (Via, via),
            (Pads, pads))
    };

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Padstack)
