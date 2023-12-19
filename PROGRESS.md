## 15 Dec.-22 Dec.
### Engine:
+ refactoring: engine/game classes separation, code clean-up
+ rendering: added OpenGL MSAA (framebuffers), fixed 2D-rendering

### Spaceship:
+ refactoring: exported variables, code clean-up, spaceship/controller separation
+ visual: added bevels on 3D models, added dynamic camera distance outline scale for spaceships (visible at far distances)
+ **3C**: 
	+ camera: tweaked aim velocity & sensibility, added dynamic FOV
	+ movement: added forward-input max throttle gain
+ UI: added 3D crosshair (w/ testing feedbacks)
+ global: shrinked world scale by ~45% (spaceships speeds were too fast)