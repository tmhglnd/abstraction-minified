// Hydra code snippets with MIDI controller assignments

setResolution(160, 90);
setResolution(480, 270);
setResolution(640, 360);
setResolution(1280, 720);
// setResolution(1920, 1080);

H.synth.time = 0;
bpm = 120;
speed = 1;

// variable controlled by pot
let v = 0.1;

// squiggle
gradient(0).pixelate(4,4)
.hue(() => 1-(v * 0.5 + 0.4))
.mask(shape(8).scale( () => v + 3, 0.26))
.rotate(0, () => v * 2).repeat(4,4)
.modulate(noise(2, 0.1), () => cos(v * pi * 4) * 2)
.out(o0)

// mosaic
osc(4,0.1,() => v * 2 + 1.7).kaleid(3)
.colorama(() => cos(v * pi * 6) * 0.1 + 0.1)  
.modulatePixelate(voronoi(5, 0.15, 0), () => sin(v * pi * 4) * 10 + 11)
.out(o0)

// smear
// src(o0)
// .modulate(src(o0).pixelate(6,6), 0.005)
// .scale(() => sin(v * pi * 4) * 0.02 + 1).hue(0.001)
// .diff(osc(2, -0.1, () => v * 2 + 3).mask(shape(2,0.02,0).scrollY(0.5)))
// .out(o0)
src(o0)
.modulate(voronoi(10, 0.1, 0), () => cos(v * 8 * pi) * 0.01)
.hue(() => sin(v * 4 * pi) * 0.005)
.scale(() => cos(v * 3 * pi) * 0.02 + 1)
.diff(osc(1, 0.4, 2.5).mask(voronoi(()=>v * 10 + 2, 0.5, 0).thresh(0.7)))
.out(o0)

// glass
// osc(2, 0.4, 0)
// .modulateKaleid(shape(() => int(v * 4) + 2, 0.2, () => sin(v * pi * 9) * 0.5 + 0.5).repeat(3), 6)
// .colorama(() => cos(v * pi * 2) * 0.01 + 7)
// .out(o0)
osc(10, 0.3).color(0, () => cos(v * pi * 3), 1)
.modulateKaleid(shape(() => int(v * 3) + 2, 0.2, 
	() => sin(v * pi * 9) * 0.5 + 0.5).repeat(11, 3), 6)
.colorama(0.4).saturate(5)
.out(o0)

// paint
noise(() => cos(v * pi * 5) * 0.2 + 0.3, 0.05)
.colorama(() => v * 0.01 + 6)
.hue(() => sin(v * pi) * -0.3)
.modulate(noise(() => cos(v * pi * 2) * 2 + 4))
.out(o0)