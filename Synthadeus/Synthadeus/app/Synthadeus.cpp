#include "Synthadeus.h"
#include "Renderables.h"

void Synthadeus::updateViewport()
{
	// update viewport movement amount with keyboard state data (false = 0 by definition)
	viewportMoveAmount[0] += inputDevice->vController.left.check() * viewportTranslateAcceleration;
	viewportMoveAmount[0] += inputDevice->vController.right.check() * -viewportTranslateAcceleration;
	viewportMoveAmount[1] += inputDevice->vController.down.check() * -viewportTranslateAcceleration;
	viewportMoveAmount[1] += inputDevice->vController.up.check() * viewportTranslateAcceleration;

	// apply friction constant
	viewportMoveAmount[0] *= viewportFriction;
	viewportMoveAmount[1] *= viewportFriction;

	// cap off the delta position
	if (viewportMoveAmount[0] > viewportMaxTranslateSpeed)
		viewportMoveAmount[0] = viewportMaxTranslateSpeed;
	if (viewportMoveAmount[0] < -viewportMaxTranslateSpeed)
		viewportMoveAmount[0] = -viewportMaxTranslateSpeed;
	if (viewportMoveAmount[1] > viewportMaxTranslateSpeed)
		viewportMoveAmount[1] = viewportMaxTranslateSpeed;
	if (viewportMoveAmount[1] < -viewportMaxTranslateSpeed)
		viewportMoveAmount[1] = -viewportMaxTranslateSpeed;

	// bring to stop if below delta espilon (whats a limit proof?)
	if (fabsf(viewportMoveAmount[0]) < viewportEpsilon)
		viewportMoveAmount[0] = 0.f;
	if (fabsf(viewportMoveAmount[1]) < viewportEpsilon)
		viewportMoveAmount[1] = 0.f;

	// move the viewport
	appWindow->viewportApplyTranslation(viewportMoveAmount);

	// if the user presses enter, reset view
	if (inputDevice->vController.center.checkPressed())
	{
		appWindow->viewportSetDefault();
		viewportMoveAmount[0] = 0.f;
		viewportMoveAmount[1] = 0.f;
	}
}

Synthadeus::Synthadeus()
	: viewportFriction(0.95f), viewportEpsilon(0.5f), viewportTranslateAcceleration(2.f),
	viewportZoomAcceleration(0.1f), viewportMaxTranslateSpeed(5.f), viewportMaxZoomSpeed(1.f)
{
	DebugPrintf("Starting Synthadeus.\n");

	// create the window and start the renderer
	appWindow = new MainWindow(this, SW_SHOWNORMAL, 1280, 768);
	appWindow->createWindow();
	DebugPrintf("Window Created.\n");

	appWindow->startRenderer();
	DebugPrintf("Renderer Started.\n");

	// create the input device
	inputDevice = new InputDevice();
	DebugPrintf("Input Device Allocated.\n");

	// create midi interface
	midiInterface = new MidiInterface();
	assert(midiInterface->initialize());
	DebugPrintf("midi successfully initialized\n");

	base = new GridBase(Point(-640.f, -640.f), Point(appWindow->getWidth() + 1280.f, appWindow->getHeight() + 1280.f), COLOR_LTGREY, COLOR_BLACK);
	audioOutputEndpoint = new AudioOutputNode(Point(appWindow->getWidth() + 340.f, appWindow->getHeight() * 0.5f + 610.f));

	audioInterface = new AudioPlayback(audioOutputEndpoint, &inputDevice->vPiano);
	assert(audioInterface->initialize());
	DebugPrintf("audio successfully initialized\n");

	base->addChild(audioOutputEndpoint);

	DebugPrintf("Base Components Initialized.\n");
}

Synthadeus::~Synthadeus()
{
	DebugPrintf("Shutting down Synthadeus.\n");

	DebugPrintf("Deinitializing Midi Interface\n");
	midiInterface->deinitialize();

	DebugPrintf("Deinitializing audio interface\n");
	audioInterface->deinitialize();

	// destroy the window and end the renderer
	appWindow->endRenderer();
	DebugPrintf("Ended Renderer.\n");
	appWindow->destroyWindow();
	delete appWindow;
	DebugPrintf("Destroyed the window.\n");

	DebugPrintf("Freeing components\n");
	base->signalRemoval();
	base->sweepDeletion();

	// free the input device
	delete midiInterface;
	delete audioInterface;
	delete inputDevice;
	delete base;
	DebugPrintf("Freed Base Components.\n");

	DebugPrintf("Synthadeus Shutdown Complete.\n");
}

void Synthadeus::run()
{
	appWindow->setRenderList(getRenderList());
	appWindow->render();
	DebugPrintf("Initial Render.\n");

	appWindow->runMessageLoop();
}

void Synthadeus::update()
{
	// APPLICATION CODE NEEDED!
	// update the input device
	inputDevice->update(midiInterface);

	// update the viewport
	updateViewport();

	inputDevice->vMouse.instance(appWindow->getViewportInstance());
	//if (inputDevice->vMouse.left.check())
	base->handleMouseInput(this, &inputDevice->vMouse);
	inputDevice->vMouse.restore();

	base->updateTree();

	base->sweepDeletion();

	if (inputDevice->vController.waveExport.checkReleased())
	{
		recalculateAudioGraph();
		WaveExporter exporter(audioOutputEndpoint->getAudioNode()->getBufferSize(), audioOutputEndpoint->getAudioNode()->getBufferL(), audioOutputEndpoint->getAudioNode()->getBufferR());
		exporter.prepareExport();
		exporter.saveWaveFile();
		exporter.unprepareExport();
	}

	if (inputDevice->vController.quit.checkReleased())
		quit();

	// reset the scroll wheel delta
	inputDevice->vMouse.scrollDelta = 0;
}

bool Synthadeus::needsRendering()
{
	// for now, we always need to render
	return true;
}

Renderable* Synthadeus::getRenderList()
{
	// create the watermark
	Renderable* watermark = new Text(SYNTHADEUS_VERSION, -1.f * appWindow->getViewportInstance(), Point((float)appWindow->getWidth(), 40.f), FONT_ARIAL40, COLOR_LTGREY);
	Renderable* renderList = sortRenderList(base->getRenderTree());
	renderList->next = watermark;

	return renderList;
}

InputDevice* Synthadeus::getInputDevice()
{
	return inputDevice;
}

void Synthadeus::quit()
{
	// give the window the quit message, terminating the realtime logic loop
	SendMessage(appWindow->getWindowHandle(), WM_QUIT, 0, 0);
}

Component* Synthadeus::findComponentAtLocation(Point pt)
{
	return base->getComponentAtPoint(pt -appWindow->getViewportInstance());
}

Renderable * Synthadeus::sortRenderList(Renderable * list)
{
	// TODO: sort the bezier curves to the end

	return list;
}

void Synthadeus::createOscillatorNode()
{
	Point place = inputDevice->vMouse.position - base->getOrigin() - appWindow->getViewportInstance();
	base->addChild(new OscillatorNode(place));
}

void Synthadeus::createEnvelopeNode()
{
	MessageBox(appWindow->getWindowHandle(), "Feature not yet fully implemented. ", "Whoops!", MB_ICONERROR);
	DebugPrintf("User requested non-existant feature: Envelope Node\n");
}

void Synthadeus::createConstantNode()
{
	MessageBox(appWindow->getWindowHandle(), "Feature not yet fully implemented. ", "Whoops!", MB_ICONERROR);
	DebugPrintf("User requested non-existant feature: Constant Node\n");
}

void Synthadeus::createMultiplierNode()
{
	MessageBox(appWindow->getWindowHandle(), "Feature not yet fully implemented. ", "Whoops!", MB_ICONERROR);
	DebugPrintf("User requested non-existant feature: Multiplier Node\n");
}

void Synthadeus::createSummationNode()
{
	Point place = inputDevice->vMouse.position - base->getOrigin() - appWindow->getViewportInstance();
	base->addChild(new SummationNode(place));
}

void Synthadeus::recalculateAudioGraph()
{
	// show a brief message while the program recalculates (could be upwards of several seconds if the graph is LARGE)
	Renderable* recalculatingMark = new Text("Calculating", -1.f * appWindow->getViewportInstance() + Point(0.f, appWindow->getHeight() / 2.f), Point((float)appWindow->getWidth(), 40.f), FONT_ARIAL40, COLOR_WHITE);
	Renderable* list = getRenderList();
	list->next->next = recalculatingMark;
	appWindow->setRenderList(list);
	appWindow->render();
	audioOutputEndpoint->getAudioNode()->recalculate();
}