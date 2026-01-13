#pragma once
#include "MainGui.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlgpu3.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#include <SDL3/SDL.h>
#include <MusicDirectoryGui.h>
#include <AppState.h>
#include <SongsTable.h>
#include <MusicPlayerGui.h>
#include <SongQueueGui.h>

void MainGui::Render(std::shared_ptr<AppState::ApplicationState> appState, std::shared_ptr<Helpers::Callbacks> callbacks)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Create SDL window graphics context
	float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL3+SDL_GPU example", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);
	if (window == nullptr)
	{
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	// Create GPU Device
	SDL_GPUDevice* gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);
	if (gpu_device == nullptr)
	{
		printf("Error: SDL_CreateGPUDevice(): %s\n", SDL_GetError());
		return;
	}

	// Claim window for GPU Device
	if (!SDL_ClaimWindowForGPUDevice(gpu_device, window))
	{
		printf("Error: SDL_ClaimWindowForGPUDevice(): %s\n", SDL_GetError());
		return;
	}
	SDL_SetGPUSwapchainParameters(gpu_device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLGPU(window);
	ImGui_ImplSDLGPU3_InitInfo init_info = {};
	init_info.Device = gpu_device;
	init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(gpu_device, window);
	init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;                      // Only used in multi-viewports mode.
	init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;  // Only used in multi-viewports mode.
	init_info.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
	ImGui_ImplSDLGPU3_Init(&init_info);
	
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool IsRunning = true;
	
	while (IsRunning)
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		// [If using SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your SDL_AppEvent() function]
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				IsRunning = false;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				IsRunning = false;
		}

		// [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate() function]
		if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
		{
			SDL_Delay(10);
			continue;
		}
		
		// (After event loop)
		// Start the Dear ImGui frame
		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		MusicDirectoryGui::Render(appState, callbacks);
		SongsTable::Render(appState, callbacks);
		MusicPlayerGui::Render(appState, callbacks);
		SongQueueGui::Render(appState, callbacks);
		//ImGui::ShowDemoWindow(); // Show demo window! :)

		// render the actual UI here

		// Rendering
		// (Your code clears your framebuffer, renders your other stuff etc.)
		ImGui::Render();
		ImDrawData* draw_data = ImGui::GetDrawData();
		const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

		SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(gpu_device); // Acquire a GPU command buffer
		SDL_GPUTexture* swapchain_texture;
		SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, nullptr, nullptr); // Acquire a swapchain texture

		if (swapchain_texture != nullptr && !is_minimized)
		{
			ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

			// Setup and start a render pass
			SDL_GPUColorTargetInfo target_info = {};
			target_info.texture = swapchain_texture;
			target_info.clear_color = SDL_FColor{ clear_color.x, clear_color.y, clear_color.z, clear_color.w };
			target_info.load_op = SDL_GPU_LOADOP_CLEAR;
			target_info.store_op = SDL_GPU_STOREOP_STORE;
			target_info.mip_level = 0;
			target_info.layer_or_depth_plane = 0;
			target_info.cycle = false;
			SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

			// Render ImGui
			ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

			SDL_EndGPURenderPass(render_pass);
		}

		// Submit the command buffer
		SDL_SubmitGPUCommandBuffer(command_buffer);
	}

	SDL_WaitForGPUIdle(gpu_device);
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplSDLGPU3_Shutdown();
	ImGui::DestroyContext();

	SDL_ReleaseWindowFromGPUDevice(gpu_device, window);
	SDL_DestroyGPUDevice(gpu_device);
	SDL_DestroyWindow(window);
	SDL_Quit();
}