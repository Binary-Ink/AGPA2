#include "Engine.h"

namespace
{
	// This is just used to forward Windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.
	DPhoenix::Engine* gd3dApp = 0;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return gd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}
namespace DPhoenix
{
	Engine::Engine(HINSTANCE hInstance)
		: mhAppInst(hInstance),
		mMainWndCaption("D3D11 Application"),		//window caption
		md3dDriverType(D3D_DRIVER_TYPE_HARDWARE),	//D3D driver type; we use hardware (GPU)
		mClientWidth(1600),							//this is the default dimensions we are using
		mClientHeight(900),
		mEnable4xMsaa(false),						//enable 4x multisampled anti-aliasing
		mhMainWnd(0),								//to be set when we have a window handle
		mAppPaused(false),							//is app paused?  false
		mMinimized(false),							//is app minimised? false
		mMaximized(false),							//is app maximiseD? false
		mResizing(false),							//are we resizing? false
		m4xMsaaQuality(0),							//quality level for 4xMSAA

		md3dDevice(0),								//here we null all D3D interface pointers
		md3dImmediateContext(0),
		mSwapChain(0),
		mDepthStencilBuffer(0),
		mRenderTargetView(0),
		mDepthStencilView(0),
													//here we null the D2D interface pointers
		mD2DFactory(0),
		mDWriteFactory(0),
		m2D1RT(0),
		mBlackBrush(0)

	{
		//clear memory for the viewport pointer
		ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));

		// Get a pointer to the application object so we can forward 
		// Windows messages to the object's window procedure through
		// the global window procedure.
		gd3dApp = this;
	}

	//destructor - release memory
	Engine::~Engine()
	{
		//if we are full screen then we need to change the mode
		//of the swapchain before release
		if (mFullScreen)
			mSwapChain->SetFullscreenState(FALSE, NULL);
		
		//release D3D interface pointers
		ReleaseCOM(mRenderTargetView);
		ReleaseCOM(mDepthStencilView);
		ReleaseCOM(mSwapChain);
		ReleaseCOM(mDepthStencilBuffer);

		// Restore all default settings.
		if (md3dImmediateContext)
			md3dImmediateContext->ClearState();

		ReleaseCOM(md3dImmediateContext);
		ReleaseCOM(md3dDevice);

		//release 2D DirectWrite Resources
		ReleaseCOM(m2D1RT);
		ReleaseCOM(mBlackBrush);
		ReleaseCOM(mD2DFactory);
	}

	//gives us the application instance -getter
	HINSTANCE Engine::AppInst()const
	{
		return mhAppInst;
	}

	//main window handle - getter
	HWND Engine::MainWnd()const
	{
		return mhMainWnd;
	}

	//works out aspect ration - getter
	float Engine::AspectRatio()const
	{
		return static_cast<float>(mClientWidth) / mClientHeight;
	}

	//main game loop
	int Engine::Run()
	{
		//reset messages so more can be listened for
		MSG msg = { 0 };

		//reset timer
		mTimer.Reset();

		//while the app is still running
		while (msg.message != WM_QUIT)
		{
			// If there are Window messages then process them.
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			// Otherwise, do animation/game stuff.
			else
			{
				//UPDATING INPUT GOES HERE
				mInput->Update();
				UpdateKeyboard();
				UpdateMouse();
				UpdateXbox();

				//update the timer
				mTimer.Tick();
				BuryEntities();

				//if we aren't paused, calculate the frames and
				//update the scene, draw the scene
				if (!mAppPaused)
				{
					
					CalculateFrameStats();
					TestForCollisions();
					UpdateScene(mTimer.DeltaTime());
					DrawScene();
				}
				else
				{
					//otherwise we'll wait for 100 milliseconds
					Sleep(100);
				}
			}
		}

		//if we close then return the message parameters
		return (int)msg.wParam;
	}

	//initialisation of D3D and core engine support
	bool Engine::Init(bool fullScreen)
	{
		//set the full screen flag based on input parameter
		mFullScreen = fullScreen;

		//try initialising main window
		if (!InitMainWindow())
			return false;

		//init direct 2d factory for font rendering
		HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&mD2DFactory
		);

		//if we have succeeded we need to create a writ facory for font rendering
		if (SUCCEEDED(hr))
		{
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&mDWriteFactory)
			);
		}

		//try initialising Direct3D
		if (!InitDirect3D())
			return false;

		//INPUT TO BE INITIALISED HERE -------------------------------------
		//initialise directinput
		debug << "Init input system" << std::endl;
		mInput = new Input(mhMainWnd);

		//create new XboxPad pointer
		mXboxInput = new XboxPad(1);

		return true;
	}

	//this is called when the window resizes (and upon initialisation)
	//when the window size changes, the buffers for the D3D device need updating to match
	//which prevents odd behaviour - hence we contain it here
	void Engine::OnResize()
	{
		//check we have our D3D interface pointers set
		assert(md3dImmediateContext);
		assert(md3dDevice);
		assert(mSwapChain);

		// Release the old views, as they hold references to the buffers we
		// will be destroying.  Also release the old depth/stencil buffer.

		ReleaseCOM(mRenderTargetView);
		ReleaseCOM(mDepthStencilView);
		ReleaseCOM(mDepthStencilBuffer);
		ReleaseCOM(m2D1RT);

		// Resize the swap chain and recreate the render target view.
		HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		
		//if we are full scren then the swap chain needs to be set as such
		if (mFullScreen)
			mSwapChain->SetFullscreenState(TRUE, NULL);

		//create the backbuffer / render target view for 3D rendering
		ID3D11Texture2D* backBuffer;
		HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
		HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
		ReleaseCOM(backBuffer);

		//backbuffer for 2d rendering (fonts)
		IDXGISurface* DXGIBackBuffer;
		// Get a surface in the swap chain
		HRESULT hr = mSwapChain->GetBuffer(
			0,
			IID_PPV_ARGS(&DXGIBackBuffer)
		);

		//(release backbuffer later as we will need it for the 2d surface)

		// Create the depth/stencil buffer and view.

		//here we create the depth buffer desc struct for rendering
		//we send the current width and height
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = mClientWidth;
		depthStencilDesc.Height = mClientHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// Use 4X MSAA? --must match swap chain MSAA values.
		if (mEnable4xMsaa)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
		}
		// No MSAA
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}

		//here we set it to be a depth stencil (what's in front, what's behind?)
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		//here we create the depth stencil and a view of it for the rendering pipeline
		HR(md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
		HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));
		
		// Bind the render target view and depth/stencil view to the pipeline.
		md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);


		// Set the viewport transform (how to position the rendering in the window).
		mScreenViewport.TopLeftX = 0;
		mScreenViewport.TopLeftY = 0;
		mScreenViewport.Width = static_cast<float>(mClientWidth);
		mScreenViewport.Height = static_cast<float>(mClientHeight);
		mScreenViewport.MinDepth = 0.0f;
		mScreenViewport.MaxDepth = 1.0f;

		md3dImmediateContext->RSSetViewports(1, &mScreenViewport);

		//setup of 2D render target for font rendering (DirectWrite)
		RECT rc;
		GetClientRect(mhMainWnd, &rc);
		//window size 
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		// Create the DXGI Surface Render Target.
		//this is for the font rendering
		FLOAT dpiX;
		FLOAT dpiY;
		mD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
				dpiX,
				dpiY
			);

		//we first check that a pointer isn't currently set before we create one
		if (!m2D1RT)
		{
			// Create a Direct2D render target.
			HRESULT  hr = mD2DFactory->CreateDxgiSurfaceRenderTarget(
				DXGIBackBuffer,
				&props,
				&m2D1RT
			);

			// Create a black brush.
			if (SUCCEEDED(hr))
			{
				hr = m2D1RT->CreateSolidColorBrush(
					D2D1::ColorF(0x000000),
					&mBlackBrush
				);
			}
		}

		//release the 2D backbuffer now we are done with it
		ReleaseCOM(DXGIBackBuffer);

	}

	//this is th engine's version of the windows message callback function
	LRESULT Engine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//check the windows message
		switch (msg)
		{
			// WM_ACTIVATE is sent when the window is activated or deactivated.  
			// We pause the game when the window is deactivated and unpause it 
			// when it becomes active.  
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				mAppPaused = true;
				mTimer.Stop();
			}
			else
			{
				mAppPaused = false;
				mTimer.Start();
			}
			return 0;

			// WM_SIZE is sent when the user resizes the window.  
		case WM_SIZE:
			// Save the new client area dimensions.
			mClientWidth = LOWORD(lParam);
			mClientHeight = HIWORD(lParam);
			if (md3dDevice)
			{
				//we set flags as necessary for the message received
				//no OnResize() call when minimised as no display necessary
				if (wParam == SIZE_MINIMIZED)
				{
					mAppPaused = true;
					mMinimized = true;
					mMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					mAppPaused = false;
					mMinimized = false;
					mMaximized = true;
					OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{
					// Restoring from minimized state?
					if (mMinimized)
					{
						mAppPaused = false;
						mMinimized = false;
						OnResize();
					}

					// Restoring from maximized state?
					else if (mMaximized)
					{
						mAppPaused = false;
						mMaximized = false;
						OnResize();
					}
					else if (mResizing)
					{
						// If user is dragging the resize bars, we do not resize 
						// the buffers here because as the user continuously 
						// drags the resize bars, a stream of WM_SIZE messages are
						// sent to the window, and it would be pointless (and slow)
						// to resize for each WM_SIZE message received from dragging
						// the resize bars.  So instead, we reset after the user is 
						// done resizing the window and releases the resize bars, which 
						// sends a WM_EXITSIZEMOVE message.
					}
					else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
					{
						OnResize();
					}
				}
			}
			return 0;

			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		case WM_ENTERSIZEMOVE:
			mAppPaused = true;
			mResizing = true;
			mTimer.Stop();
			return 0;

			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:
			mAppPaused = false;
			mResizing = false;
			mTimer.Start();
			OnResize();
			return 0;

			// WM_DESTROY is sent when the window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

			// The WM_MENUCHAR message is sent when a menu is active and the user presses 
			// a key that does not correspond to any mnemonic or accelerator key. 
		case WM_MENUCHAR:
			// Don't beep when we alt-enter.
			return MAKELRESULT(0, MNC_CLOSE);

			// Catch this message so to prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;

		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	//here we initialise the main window
	bool Engine::InitMainWindow()
	{
		//window class struct
		WNDCLASS wc;

		//here we set up key calues / properties for the window
		wc.style = CS_HREDRAW | CS_VREDRAW;				//allow horiz / vertical redrawing / resizing
		wc.lpfnWndProc = MainWndProc;					//callback function name
		wc.cbClsExtra = 0;								//any extra bytes for the struct?
		wc.cbWndExtra = 0;								//additional bytes for window instance?
		wc.hInstance = mhAppInst;						//handle to application instance
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);		//are we loading an app icon? (taskbar etc.)
		wc.hCursor = LoadCursor(0, IDC_ARROW);			//cursor to load
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);		//background drawing
		wc.lpszMenuName = 0;							//any menu to load
		wc.lpszClassName = "D3DWndClassName";			//name of window class

		//error checking for registering the window class
		if (!RegisterClass(&wc))
		{
			MessageBox(0, "RegisterClass Failed.", 0, 0);
			return false;
		}

		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT R = { 0, 0, mClientWidth, mClientHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		//create the window
		mhMainWnd = CreateWindow("D3DWndClassName",			//window class name
								mMainWndCaption.c_str(),	//window caption
								WS_OVERLAPPEDWINDOW,		//window style
								CW_USEDEFAULT,				//x pos
								CW_USEDEFAULT,				//y pos
								width,						//width
								height,						//height
								0,							//parent window
								0,							//menu
								mhAppInst,					//application instance handle
								0);							//any parameters to pass in
		//error checking
		if (!mhMainWnd)
		{
			MessageBox(0, "CreateWindow Failed.", 0, 0);
			return false;
		}

		//show and update window
		ShowWindow(mhMainWnd, SW_SHOW);
		UpdateWindow(mhMainWnd);

		return true;
	}

	//initialise the Direct3D interfaces
	bool Engine::InitDirect3D()
	{
		// Create the device and device context.
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; //for 2d rendering / font
		//if in debug build we also set debug flags
		#if defined(DEBUG) || defined(_DEBUG)  
				createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		//the feature level is what is supporte don the platform
		D3D_FEATURE_LEVEL featureLevel;
		HRESULT hr = D3D11CreateDevice(
			0,						// default adapter
			md3dDriverType,			//driver type (we set hardware / GPU)
			0,						// no software device
			createDeviceFlags,		//defined abov - flags for device creation
			0, 0,					// default feature level array
			D3D11_SDK_VERSION,		//SDK version (Direct3D 11)
			&md3dDevice,			//output pointer for created device
			&featureLevel,			//output pointer for feature levels supported
			&md3dImmediateContext);	//output pointer for device context

		//did it work? if not quit and message user
		if (FAILED(hr))
		{
			MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
			return false;
		}

		//if Direct3D 11 not supported lets go no further
		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		// Check 4X MSAA quality support for our back buffer format.
		// All Direct3D 11 capable devices support 4X MSAA for all render 
		// target formats, so we only need to check quality support.
		HR(md3dDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
		assert(m4xMsaaQuality > 0);

		// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
		DXGI_SWAP_CHAIN_DESC sd;
		//width and height
		sd.BufferDesc.Width = mClientWidth;
		sd.BufferDesc.Height = mClientHeight;
		//frames per second (60/1)
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		//format flags
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;

		// Use 4X MSAA? 
		if (mEnable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m4xMsaaQuality - 1;
		}
		// No MSAA
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		//we are going to use this with / as a render target
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = mhMainWnd;
		sd.Windowed = TRUE;
		//discard back buffer contents after call to Present
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//lets us be fullscreen

		// To correctly create the swap chain, we must use the IDXGIFactory that was
		// used to create the device.  If we tried to use a different IDXGIFactory instance
		// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
		// This function is being called with a device from a different IDXGIFactory."

		IDXGIDevice* dxgiDevice = 0;
		HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

		IDXGIAdapter* dxgiAdapter = 0;
		HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

		IDXGIFactory* dxgiFactory = 0;
		HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

		HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));

		//release temp object pointers - we don't need thm anymore
		ReleaseCOM(dxgiDevice);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiFactory);

		// The remaining steps that need to be carried out for d3d creation
		// also need to be executed every time the window is resized.  So
		// just call the OnResize method here to avoid code duplication.
		OnResize();

		return true;
	}

	//calculae the frames for logic / rendering
	void Engine::CalculateFrameStats()
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.  These stats 
		// are appended to the window caption bar.
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;

		frameCnt++;

		// Compute averages over one second period.
		if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
		{
			float fps = (float)frameCnt; // fps = frameCnt / 1
			float mspf = 1000.0f / fps;

			std::ostringstream outs;
			outs.precision(6);
			outs << mMainWndCaption << "    "
				<< "FPS: " << fps << "    "
				<< "Frame Time: " << mspf << " (ms)";
			SetWindowText(mhMainWnd, outs.str().c_str());

			// Reset for next average.
			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}

	void Engine::UpdateKeyboard()
	{
		//number of keys
		const int KEYS = 256;
		//array to hold the keys
		static char oldKeys[KEYS];

		for (int n = 0; n < KEYS; n++)
		{
			//check for key press
			if (mInput->GetKeyState(n) & 0x80)
			{
				//store current keystate (on)
				oldKeys[n] = mInput->GetKeyState(n);
				//launch event
				RaiseEvent(new KeyPressEvent(n));
			}
			//check for release (is in old keys but not currently pressed)
			else if (oldKeys[n] & 0x80)
			{
				//store current keystate (off)
				oldKeys[n] = mInput->GetKeyState(n);
				//launch event
				RaiseEvent(new KeyReleaseEvent(n));
			}
		}
	}

	void Engine::UpdateMouse()
	{
		static int oldPosX = 0;
		static int oldPosY = 0;

		//check mouse position
		int posx = mInput->GetMousePosX();
		int posy = mInput->GetMousePosY();
		if (posx != oldPosX || posy != oldPosY)
		{
			int oldPosX = mInput->GetMousePosX();
			int oldPosY = mInput->GetMousePosY();
			//launch event
			RaiseEvent(new MouseMoveEvent(posx, posy));
		}

		//check mouse motion
		int deltax = mInput->GetMouseDeltaX();
		int deltay = mInput->GetMouseDeltaY();
		if (deltax != 0 || deltay != 0)
		{
			//launch event
			RaiseEvent(new MouseMotionEvent(deltax, deltay));
		}

		//check mouse buttons
		for (int n = 0; n<4; n++)
		{
			if (mInput->GetMouseButton(n))
			{
				//launch event
				RaiseEvent(new MouseClickEvent(n, posx, posy));
			}
		}

		//check mouse wheel
		int wheel = mInput->GetMouseDeltaWheel();
		if (wheel != 0)
		{
			//launch event
			RaiseEvent(new MouseWheelEvent(wheel));
		}
	}

	void Engine::RaiseEvent(IEvent* e)
	{
		//call event handler method
		HandleEvents(e);
		delete e;
	}

	void Engine::UpdateXbox()
	{
		XINPUT_STATE PadState = mXboxInput->GetState();		//get current pad state
		XboxPad* padPtr = mXboxInput;						//set pointer to pad
		RaiseEvent(new XBoxEvent(PadState, padPtr));		//raise event
	}

	//add entity pointer to global entity STL
	void Engine::AddEntity(Entity * entity)
	{
		//log text
		debug << "Engine::addEntity: received new entity\n";
		//counter for unique ids
		static int id = 0;
		//set entity ID
		entity->mId = id;
		//add entity to vector
		mEntities.push_back(entity);
		//increment id
		id++;
	}

	//count entities of a given type
	int Engine::GetEntityCount(EntityType entityType)
	{
		//reset counter
		int count = 0;
		//loop through entities
		for each (Entity* entity in mEntities)
		{
			//if the entity type matches, increment count
			if (entity->mEntityType == entityType)
				count++;
		}
		//return final count
		return count;
	}

	//give an entity id and return the matching entity (if exists)
	Entity * Engine::FindEntity(int id)
	{
		//loop through and return alive entity where name matches
		for each (Entity* entity in mEntities)
		{
			//check id matches, if so return entity pointer
			if (entity->mAlive && entity->mId == id)
				return entity;
		}
		//if here no id matches, return null
		return NULL;
	}

	//find an entity by it's name
	Entity * Engine::FindEntity(std::string name)
	{
		//loop through and return alive entity where name matches
		for each (Entity* entity in mEntities)
		{
			//check name matches, if so return entity pointer
			if (entity->mAlive && entity->mName == name)
				return entity;
		}
		//if here no id matches, return null
		return NULL;
	}

	//this clears the memory for all entities in the engine
	//(referencing becomes important; is it deleted from other classes
	//referencing the entity subtype first?)
	void Engine::BuryEntities()
	{
		//an iterator will work better than for each in this case
		std::vector<Entity*>::iterator iter = mEntities.begin();
		while (iter != mEntities.end())
		{
			//if entity not alive, erase from vector
			if ((*iter)->mAlive == false)
			{
				iter = mEntities.erase(iter);
			}
			//otherwise increment iterator
			else iter++;
		}
	}

	//main collision testing method - calls main checking cases
	//for two collidable / alive entities
	void Engine::TestForCollisions()
	{
		//reset all collided properties
		for each(Entity* entity in  mEntities)
		{
			entity->mCollided = false;
			entity->mCollideBuddy = NULL;

			//ADDED FOR MELEE / BEACONS
			entity->mAABBCollidedEntities.clear();

		}

		//escape if global collisions are disabled
		if (!mGlobalCollisionOn) return;

		//loop through all entities (first)
		for each(Entity* first in mEntities)
		{
			if (first->mAlive && first->mCollidable)
			{
				//test all other entities for collision (second)
				for each(Entity* second in mEntities)
				{
					//do not test object with itself
					if (second->mId != first->mId)
					{

						if (second->mAlive && second->mCollidable)
						{
							//test for collision
							if (Collision(first, second))
							{
								//set collision flags and buddies
								first->mCollided = true;
								//first->mCollidedEntities.push_back(second);
								second->mCollided = true;
								//second->mCollidedEntities.push_back(first);
							}
						}//if
					}//if
				}//foreach
			}//if
		}//foreach
	}

	//Collision checking method, takes two entity types
	bool Engine::Collision(Entity * entity1, Entity * entity2)
	{
		switch (entity1->mEntityType)
		{
			//THIS CHECK ALLOWS US TO DETERMINE THAT THE FIRST ENTITY IS A PLAYER
			case ENTITY_PLAYER_MESH:
				switch (entity2->mEntityType)
				{
					//if second one a scenery mesh
				case ENTITY_SCENERY_MESH:
					return AABBCollision(entity1, entity2, true, false, false);
					break;
					//if second one is collectable mesh
				case ENTITY_COLLECTABLE_MESH:
					return AABBCollision(entity1, entity2, false, false, false);
					break;
				case ENTITY_ENEMY_MESH:
					return AABBCollision(entity1, entity2, false, false, true);
					break;
				case ENTITY_BEACON_MESH:
					return AABBCollision(entity1, entity2, false, false, true);
					break;
				}
			break;
			case ENTITY_CAMERA_LINE:
				switch (entity2->mEntityType)
				{
					//if second one a scenery mesh
				case ENTITY_SELECTION:
					return LineAABBCollision((Line*)entity1, entity2);
					break;

				}
			break;
			case ENTITY_WEAPON_LINE:
				switch (entity2->mEntityType)
				{
					//if second one a scenery mesh
				case ENTITY_ENEMY_MESH:
				case ENTITY_PLAYER_MESH:
				case ENTITY_COVER_MESH:
				case ENTITY_WALL_MESH:
					return LineAABBCollision((Line*)entity1, entity2);
					break;

				}
			break;
			case ENTITY_WEAPON_MESH:
				switch (entity2->mEntityType)
				{
					//if second one a scenery mesh
				case ENTITY_ENEMY_MESH:
				case ENTITY_PLAYER_MESH:
				case ENTITY_COVER_MESH:
				case ENTITY_WALL_MESH:
					return SphereAABBCollision(entity1, entity2, true);
					break;

				}
			break;
			case ENTITY_ENEMY_MESH:
				switch (entity2->mEntityType)
				{
					//if second one a scenery mesh				
				case ENTITY_BEACON_MESH:
					return AABBCollision(entity1, entity2, false, false, true);
					break;
				}
				break;
			case ENTITY_CAMERA_COLLISION:
				switch (entity2->mEntityType)
				{
					//if second one a scenery mesh				
				case ENTITY_BEACON_MESH:
				case ENTITY_ENEMY_MESH:
				case ENTITY_PLAYER_MESH:
				case ENTITY_COVER_MESH:
				case ENTITY_WALL_MESH:
				case ENTITY_SELECTION:
					return AABBCollision(entity1, entity2, false, true, false);
					break;
				}
				break;
		}
		
		return false;
	}

	//Axis-Aligned Bounding Box collision checks
	bool Engine::AABBCollision(Entity * entity1, Entity * entity2, bool isSolid, bool isCamera, bool isStored)
	{
		//min and max points for both boxes to test
		XMFLOAT3 maxBox1; XMFLOAT3 minBox1;
		XMFLOAT3 maxBox2; XMFLOAT3 minBox2;

		//by default, turn off camera collided flag
		//if collided with camera we need to set the flag for this update
		if (isCamera)
		{
			entity2->mCollidedWithCamera = false;
		}

		//if the distance in position is greater than the halfsizes on that axis
		//then no collision

		//test x axis
		if (abs(entity1->mPosition.x - entity2->mPosition.x) >
			entity1->mHalfSizes.x + entity2->mHalfSizes.x) return false;
		//test y axis
		if (abs(entity1->mPosition.y - entity2->mPosition.y) >
			entity1->mHalfSizes.y + entity2->mHalfSizes.y) return false;
		//test z axis
		if (abs(entity1->mPosition.z - entity2->mPosition.z) >
			entity1->mHalfSizes.z + entity2->mHalfSizes.z) return false;

		//if solid then we need to return the objects to their previous position
		if (isSolid)
		{
			entity1->mPosition = entity1->mPrevPosition;
			entity2->mPosition = entity2->mPrevPosition;
		}

		//if collided with camera we need to set the flag for this update
		if (isCamera)
		{
			entity2->mCollidedWithCamera = true;
		}

		//need some checks for ENEMY / PLAYER / BEACON meshes for 'touch' mechanics
		if (isStored)
		{
			entity1->mAABBCollidedEntities.push_back(entity2);
			entity2->mAABBCollidedEntities.push_back(entity1);
		}

		//flag true collision
		return true;
	}


	bool Engine::LineAABBCollision(Line * line, Entity * box)
	{
		//line-aabb intersection test
		int hasHit;
		float distanceToCollision;

		//update bounding box values
		XMFLOAT3 maxBox; XMFLOAT3 minBox;

		maxBox.x = box->mPosition.x + box->mHalfSizes.x;
		maxBox.y = box->mPosition.y + box->mHalfSizes.y;
		maxBox.z = box->mPosition.z + box->mHalfSizes.z;

		minBox.x = box->mPosition.x - box->mHalfSizes.x;
		minBox.y = box->mPosition.y - box->mHalfSizes.y;
		minBox.z = box->mPosition.z - box->mHalfSizes.z;

		//line properties
		XMFLOAT3 lineBegin = line->mLineBegin;
		XMFLOAT3 lineEnd = line->mLineEnd;

		//fraction of line remaining 
		float fractionLow = 0.0f;
		float fractionHigh = 1.0f;

		//CLIP X
		//x distance from box min/max to line begin / x distance
		float distVar = lineEnd.x - lineBegin.x;
		if (distVar == 0.0f) distVar = 1.0f;

		float fractionLowX = (minBox.x - lineBegin.x) / distVar;
		float fractionHighX = (maxBox.x - lineBegin.x) / distVar;

		//ensure the high / low valus are correct
		if (fractionHighX < fractionLowX)
			std::swap(fractionLowX, fractionHighX);

		//if the max x clip is less than 0, no intersection
		if (fractionHighX < fractionLow)
			return false;

		//if the min x clip is greater than 1, no intersection
		if (fractionLowX > fractionHigh)
			return false;

		//reset the low and high fractions of the line to
		//the min and max x clip (clip the line)
		fractionLow = max(fractionLowX, fractionLow);
		fractionHigh = min(fractionHighX, fractionHigh);

		//if the low clip is greater than the high clip, no intersection
		if (fractionLow > fractionHigh)
			return false;

		//CLIP Y
		//y distance from box min/max to line begin / y distance
		distVar = lineEnd.y - lineBegin.y;
		if (distVar == 0.0f) distVar = 1.0f;

		float fractionLowY = (minBox.y - lineBegin.y) / distVar;
		float fractionHighY = (maxBox.y - lineBegin.y) / distVar;

		//ensure the high / low values are correct
		if (fractionHighY < fractionLowY)
			std::swap(fractionLowY, fractionHighY);

		//if the max y clip is less than low clip, no intersection
		if (fractionHighY < fractionLow)
			return false;

		//if the min y clip is greater than high clip, no intersection
		if (fractionLowY > fractionHigh)
			return false;

		//reset the low and high fractions of the line to
		//the min and max x clip (clip the line)
		fractionLow = max(fractionLowY, fractionLow);
		fractionHigh = min(fractionHighY, fractionHigh);

		//if the low clip is greater than the high clip, no intersection
		if (fractionLow > fractionHigh)
			return false;

		//CLIP Z
		//z distance from box min/max to line begin / z distance
		distVar = lineEnd.z - lineBegin.z;
		if (distVar == 0.0f) distVar = 1.0f;

		float fractionLowZ = (minBox.z - lineBegin.z) / distVar;
		float fractionHighZ = (maxBox.z - lineBegin.z) / distVar;

		//ensure the high / low valus are correct
		if (fractionHighZ < fractionLowZ)
			std::swap(fractionLowZ, fractionHighZ);

		//if the max x clip is less than low clip, no intersection
		if (fractionHighZ < fractionLow)
			return false;

		//if the min x clip is greater than high clip, no intersection
		if (fractionLowZ > fractionHigh)
			return false;

		//reset the low and high fractions of the line to
		//the min and max x clip (clip the line)
		fractionLow = max(fractionLowZ, fractionLow);
		fractionHigh = min(fractionHighZ, fractionHigh);

		//if the low clip is greater than the high clip, no intersection
		if (fractionLow > fractionHigh)
			return false;

		//-----------------------------------------------------------

		//here we have an intersection point!
		//get the full line length
		float intersectionDistance = line->mLineSize * fractionLow;
		//get the intersection point (the low clip)
		XMFLOAT3 intersectionPoint;
		intersectionPoint.x = lineBegin.x + (lineEnd.x - lineBegin.x) * fractionLow;
		intersectionPoint.y = lineBegin.y + (lineEnd.y - lineBegin.y) * fractionLow;
		intersectionPoint.z = lineBegin.z + (lineEnd.z - lineBegin.z) * fractionLow;

		line->AddCollision(intersectionDistance, intersectionPoint, box);

		//collision true
		return true;
	}

	//an implemnetation of Arvo's algorithm
	bool Engine::SphereAABBCollision(Entity * sphere, Entity * box, bool collideOnce)
	{
		//we need to get the 'squared distance' (distance^2) between
		//centre of sphere and the AABB (box) 
		
		//update bounding box values
		XMFLOAT3 maxBox; XMFLOAT3 minBox;
		maxBox.x = box->mPosition.x + box->mHalfSizes.x;
		maxBox.y = box->mPosition.y + box->mHalfSizes.y;
		maxBox.z = box->mPosition.z + box->mHalfSizes.z;

		minBox.x = box->mPosition.x - box->mHalfSizes.x;
		minBox.y = box->mPosition.y - box->mHalfSizes.y;
		minBox.z = box->mPosition.z - box->mHalfSizes.z;


		float sqDist = 0.0f;
		// for each axis count any excess distance outside box extents
		//x axis test
		float x = sphere->mPosition.x;
		if (x < minBox.x) sqDist += (minBox.x - x) * (minBox.x - x);
		if (x > maxBox.x) sqDist += (x - maxBox.x) * (x - maxBox.x);
		//y axis test
		float y = sphere->mPosition.y;
		if (y < minBox.y) sqDist += (minBox.y - y) * (minBox.y - y);
		if (y > maxBox.y) sqDist += (y - maxBox.y) * (y - maxBox.y);
		//z axis test
		float z = sphere->mPosition.z;
		if (z < minBox.z) sqDist += (minBox.z - z) * (minBox.z - z);
		if (z > maxBox.z) sqDist += (z - maxBox.z) * (z - maxBox.z);

		bool collision = false;

		// Sphere and AABB intersect if the (squared) distance between them is
		// less than the (squared) sphere radius.
		//(we expect the halfsizes to be the same on each axis to represent radius)
		if (abs(sqDist) <= sphere->mHalfSizes.x * sphere->mHalfSizes.x)
		{
			collision = true;
		}

		//add to collection of collided entities if set to only collide once and not collided yet
		if (collideOnce && collision)
		{
			bool hasCollidedAlready = false;
			for (int i = 0; i < sphere->mCollidedEntities.size(); i++)
			{
				if (box->mId == sphere->mCollidedEntities[i]->mId)
					hasCollidedAlready = true;
			}
			if (!hasCollidedAlready)
				sphere->mCollidedEntities.push_back(box);
		}

		return collision;
	}
}

