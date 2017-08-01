#pragma once

#include <mbgl/tile/tile.hpp>
#include <mbgl/tile/tile_loader.hpp>
#include <mbgl/tile/raster_tile_worker.hpp>
#include <mbgl/actor/actor.hpp>

namespace mbgl {

class Tileset;
class TileParameters;
class RasterBucket;

namespace style {
class Layer;
} // namespace style

class RasterTile : public Tile {
public:
    RasterTile(const OverscaledTileID&,
                   const TileParameters&,
                   const Tileset&);
    ~RasterTile() final;

    void setNecessity(Necessity) final;

    void setError(std::exception_ptr, bool complete);
    void setData(optional<std::shared_ptr<const std::string>> data,
                 optional<Timestamp> modified_,
                 optional<Timestamp> expires_,
                 bool complete);

    void cancel() override;

    void upload(gl::Context&) override;
    Bucket* getBucket(const style::Layer::Impl&) const override;

    void setMask(TileMask&&) override;

    void onParsed(std::unique_ptr<RasterBucket> result, uint64_t resultCorrelationID);
    void onError(std::exception_ptr, uint64_t resultCorrelationID);

private:
    TileLoader<RasterTile> loader;

    std::shared_ptr<Mailbox> mailbox;
    Actor<RasterTileWorker> worker;

    uint64_t correlationID = 0;

    // Contains the Bucket object for the tile. Buckets are render
    // objects and they get added by tile parsing operations.
    std::unique_ptr<RasterBucket> bucket;
};

} // namespace mbgl

