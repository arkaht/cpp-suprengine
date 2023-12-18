#pragma once

#include <suprengine/render-batch.h>

namespace suprengine
{
	class Engine;

	struct GameInfos
	{
		std::string title;
		int width;
		int height;
	};

	class IGame
	{
	public:
		//IGame() = delete;
		virtual ~IGame() {};

		virtual void load_assets() = 0;

		virtual void init() = 0;
		virtual void release() = 0;

		virtual GameInfos get_infos() const = 0;

		virtual Engine* get_engine() const = 0;
	};

	template <typename TRenderBatch>
	class Game : public IGame
	{
		static_assert( 
			std::is_base_of<RenderBatch, TRenderBatch>::value, 
			"Game: use of an invalid RenderBatch class!" 
		);

	public:
		Game() {}
		Engine* get_engine() const { return _engine; }
		TRenderBatch* get_render_batch() const { return _render_batch; }
		
	private:
		TRenderBatch* _render_batch { nullptr };
		Engine* _engine { nullptr };

		TRenderBatch* _create_render_batch( Window* window )
		{
			_render_batch = new TRenderBatch( window );
			return _render_batch;
		}

		friend class Engine;
	};
}