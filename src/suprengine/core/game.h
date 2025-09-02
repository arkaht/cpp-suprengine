#pragma once

#include "render-batch.h"

namespace suprengine
{
	class Engine;

	/*
	 * Basic informations given to the engine by the game.
	 */
	struct GameInfos
	{
		WindowInfos window;
	};

	class IGame
	{
	public:
		virtual ~IGame() {};

		virtual void load_assets() = 0;

		virtual void init() = 0;
		virtual void release() = 0;

		virtual RenderBatch* create_render_batch( Window* window ) = 0;

		virtual GameInfos get_infos() const = 0;

		virtual void set_engine( Engine* ) = 0;
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

		RenderBatch* create_render_batch( Window* window ) override
		{
			_render_batch = new TRenderBatch( window );
			return _render_batch;
		}

		void set_engine( Engine* engine ) override { _engine = engine; };

		Engine* get_engine() const override { return _engine; }
		TRenderBatch* get_render_batch() const { return _render_batch; }
		
	private:
		TRenderBatch* _render_batch { nullptr };
		Engine* _engine { nullptr };

		friend class Engine;
	};
}