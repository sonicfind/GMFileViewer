#include "dxerr.h"
#include "GraphicsThrowMacros.h"
#include "Graphics_DX.h"

#pragma comment(lib, "d3d11.lib")

namespace wrl = Microsoft::WRL;

Graphics_DX::Graphics_DX(HINSTANCE hInstance)
    : Graphics("window", 1280, 960, 300)
    , modelVertex("ModelVertex.cso")
    , modelPixel("ModelPixel.cso")
    , shadowVertex("ShadowVertex.cso")
    , shadowGeometry("ShadowGeometry.cso")
    , shadowPixel("ShadowPixel.cso")
    , spriteVertex("SpriteVertex.cso")
    , spriteGeometry("SpriteGeometry.cso")
    , spritePixel("SpritePixel.cso")
    , boxVertex("BoxVertex.cso")
    , boxPixel("BoxPixel.cso")
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.hInstance = hInstance;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = nullptr;
    wc.hCursor = nullptr;
    wc.hIcon = nullptr;
    wc.hIconSm = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = wndClassName;
    RegisterClassEx(&wc);

    RECT wr = { 0 };
    wr.left = 360;
    wr.right = m_settings.width + wr.left;
    wr.top = 360;
    wr.bottom = m_settings.height + wr.top;

    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
        throw GMWND_LAST_EXCEPT();

    // create window
    hWnd = CreateWindow(
        wndClassName, m_settings.name.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, hInstance, this
    );

    if (hWnd == nullptr)
        throw GMWND_LAST_EXCEPT();

    DXGI_SWAP_CHAIN_DESC sd = { 0 };
    // Uses window size
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    // Use default
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    // No scaling (for now)
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    // Anti-aliasing
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    HRESULT hr;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwap,
        &pDevice,
        nullptr,
        &pContext
    ));

    wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
    GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
    GFX_THROW_INFO(pDevice->CreateRenderTargetView(
        pBackBuffer.Get(),
        nullptr,
        &pTarget
    ));

    D3D11_RASTERIZER_DESC raster_desc = {
        D3D11_FILL_SOLID,
        D3D11_CULL_NONE,
        TRUE,
        0,0,0,
        TRUE,
        FALSE,
        FALSE,
        FALSE
    };

    raster_desc.FrontCounterClockwise = TRUE;
    raster_desc.CullMode = D3D11_CULL_NONE;
    pDevice->CreateRasterizerState(&raster_desc, &rasterStates[(int)FrontFace::Countrclockwise][(int)Culling::DISABLED]);
    raster_desc.CullMode = D3D11_CULL_BACK;
    pDevice->CreateRasterizerState(&raster_desc, &rasterStates[(int)FrontFace::Countrclockwise][(int)Culling::BACK]);
    raster_desc.CullMode = D3D11_CULL_FRONT;
    pDevice->CreateRasterizerState(&raster_desc, &rasterStates[(int)FrontFace::Countrclockwise][(int)Culling::FRONT]);
    raster_desc.FrontCounterClockwise = FALSE;
    raster_desc.CullMode = D3D11_CULL_NONE;
    pDevice->CreateRasterizerState(&raster_desc, &rasterStates[(int)FrontFace::Clockwise][(int)Culling::DISABLED]);
    raster_desc.CullMode = D3D11_CULL_BACK;
    pDevice->CreateRasterizerState(&raster_desc, &rasterStates[(int)FrontFace::Clockwise][(int)Culling::BACK]);
    raster_desc.CullMode = D3D11_CULL_FRONT;
    pDevice->CreateRasterizerState(&raster_desc, &rasterStates[(int)FrontFace::Clockwise][(int)Culling::FRONT]);
}

LRESULT WINAPI Graphics_DX::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // use create param passed in from CreateWindow() to store window class pointer
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Graphics_DX* const pGraphics = static_cast<Graphics_DX*>(pCreate->lpCreateParams);
        // Set WinAPI-managed user data to store ptr to window
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pGraphics));
        // set message proc to normal handler
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&HandleMsgThunk));
        // forward message
        return pGraphics->HandleMsg(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Graphics_DX::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // retrieve ptr to window class
    Graphics_DX* const pGraphics = reinterpret_cast<Graphics_DX*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pGraphics->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Graphics_DX::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        break;
    case WM_KEYUP:
        break;
    case WM_CHAR:
        break;
    case WM_MOUSEMOVE:
        if ((wParam & MK_LBUTTON) == 0)
            break;
        __fallthrough;
    case WM_LBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        std::ostringstream oss;
        oss << '(' << pt.x << ',' << pt.y << ')';
        SetWindowText(hWnd, oss.str().c_str());
    }
    break;
    case WM_LBUTTONUP:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Graphics_DX::activateShader(ShaderType type) const
{
    switch (type)
    {
    case Graphics::Model:
        pContext->VSSetShader(modelVertex.Get(), nullptr, 0);
        pContext->GSSetShader(nullptr, nullptr, 0);
        pContext->PSSetShader(modelPixel.Get(), nullptr, 0);
        break;
    case Graphics::Model_NoDepth:
        break;
    case Graphics::Shadow:
        pContext->VSSetShader(shadowVertex.Get(), nullptr, 0);
        pContext->GSSetShader(shadowGeometry.Get(), nullptr, 0);
        pContext->PSSetShader(shadowPixel.Get(), nullptr, 0);
        break;
    case Graphics::Sprite:
        pContext->VSSetShader(spriteVertex.Get(), nullptr, 0);
        pContext->GSSetShader(spriteGeometry.Get(), nullptr, 0);
        pContext->PSSetShader(spritePixel.Get(), nullptr, 0);
        break;
    case Graphics::Box:
        pContext->VSSetShader(boxVertex.Get(), nullptr, 0);
        pContext->GSSetShader(nullptr, nullptr, 0);
        pContext->PSSetShader(boxPixel.Get(), nullptr, 0);
        break;
    default:
        break;
    }
}

void Graphics_DX::setShaderInt(const char* name, int value) const
{
}

size_t Graphics_DX::createVertexBuffer(ShaderType type, const void* data, uint32_t dataSize, bool isDynamic)
{
    D3D11_BUFFER_DESC description = VertexBuffer::DESCRIPTION_base;
    description.ByteWidth = dataSize;
    description.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;

    const D3D11_INPUT_ELEMENT_DESC* inputDesc;
    const FileContainer* shaderFile;
    //const 
    switch (type)
    {
    case Graphics::Model:
    {
        static constexpr size_t VERTSIZE_MODEL = 52;
        static constexpr D3D11_INPUT_ELEMENT_DESC MODELDESC[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,                  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 4  * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 7  * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 11 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        description.StructureByteStride = VERTSIZE_MODEL;
        inputDesc = MODELDESC;
        shaderFile = &modelVertex.Data();
    }
    break;
    case Graphics::Shadow:
    {
        static constexpr size_t VERTSIZE_SHADOW = 0;
        static constexpr D3D11_INPUT_ELEMENT_DESC SHADOWDESC[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,                  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        description.StructureByteStride = VERTSIZE_SHADOW;
        inputDesc = SHADOWDESC;
        shaderFile = &shadowVertex.Data();
    }
    break;
    case Graphics::Sprite:
    {
        static constexpr D3D11_INPUT_ELEMENT_DESC SPRITEDESC[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,                  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        inputDesc = SPRITEDESC;
        shaderFile = &spriteVertex.Data();
    }
    break;
    case Graphics::Box:
    {
        static constexpr size_t VERTSIZE_BOX = 16;
        static constexpr D3D11_INPUT_ELEMENT_DESC BOXDESC[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,                  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 2 * sizeof(float),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        description.StructureByteStride = VERTSIZE_BOX;
        inputDesc = BOXDESC;
        shaderFile = &boxVertex.Data();
    }
    break;
    default:
        return SIZE_MAX;
    }

    vertexBuffers.emplace_back(*pDevice.Get(), description, data, inputDesc, *shaderFile);
    return vertexBuffers.size() - 1;
}

void Graphics_DX::setActiveVertexBuffer(size_t index)
{
    bufferToUpdate = vertexBuffers[index].Get();
}

void Graphics_DX::bindVertexBuffer(size_t index) const
{
    vertexBuffers[index].bind(*pContext.Get());
}

void Graphics_DX::updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
    UpdateBuffer(offset, data, dataSize);
}

size_t Graphics_DX::createIndexBuffer(const void* data, uint32_t dataSize)
{
    D3D11_BUFFER_DESC description = IndexBuffer::DESCRIPTION_base;
    description.ByteWidth = dataSize;
    description.Usage = D3D11_USAGE_IMMUTABLE;
    description.StructureByteStride = sizeof(uint32_t);

    indexBuffers.emplace_back(*pDevice.Get(), description, data);
    return indexBuffers.size() - 1;
}

void Graphics_DX::bindIndexBuffer(size_t index) const
{
    pContext->IASetIndexBuffer(indexBuffers[index].Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Graphics_DX::createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height)
{
}

void Graphics_DX::bindTexture(std::string_view name) const
{
}

void Graphics_DX::updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const
{
}

void Graphics_DX::unbindTexture() const
{
}

void Graphics_DX::selectSkyTexture(std::string_view name)
{
}

void Graphics_DX::setActiveTexture(uint32_t slot) const
{
}

void Graphics_DX::bindConstantBuffer(ConstBufferSelection selection) const
{
}

void Graphics_DX::updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
}

void Graphics_DX::setFrontFace(FrontFace front)
{
    frontFace = front;
    SetRasterizerState();
}

void Graphics_DX::setCullFunc(Culling cull)
{
    culling = cull;
    SetRasterizerState();
}

void Graphics_DX::enable(Enablable property) const
{
}

void Graphics_DX::disable(Enablable property) const
{
}

void Graphics_DX::setDepthFunc(DepthTest testParam) const
{
}

void Graphics_DX::setBlendFunc(Blending blend) const
{
}

void Graphics_DX::setClearColor(float r, float g, float b, float a) const
{
    clearColor[0] = r;
    clearColor[1] = g;
    clearColor[2] = b;
    clearColor[3] = a;
}

void Graphics_DX::setTopology(PrimitiveMode mode) const
{
    pContext->IASetPrimitiveTopology(mode == PrimitiveMode::TRIANGLE_STRIP ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics_DX::drawArrays(uint32_t index, uint32_t count) const
{
    pContext->Draw(count, index);
}

void Graphics_DX::drawElements(uint32_t count, size_t offset) const
{
    pContext->DrawIndexed(count, (uint32_t)offset, 0);
}

uint32_t Graphics_DX::addIgnorableDepthTexture()
{
    return 0;
}

void Graphics_DX::copyDepthToIgnorable(uint32_t id) const
{
}

void Graphics_DX::setDepthlessDepthTextures(uint32_t id) const
{
}

void Graphics_DX::resetFrame() const
{
    pContext->ClearRenderTargetView(pTarget.Get(), clearColor);
}

void Graphics_DX::displayFrame() const
{
    MSG msg = { 0 };
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            ecode = (int)msg.wParam;
            return;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    HRESULT hr;
#ifndef NDEBUG
    infoManager.Set();
#endif

    if (FAILED(hr = pSwap->Present(1, 0)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        throw GFX_EXCEPT(hr);
    }
}

void Graphics_DX::updateTitle(const std::string& title) const
{
    if (SetWindowText(hWnd, title.c_str()) == 0)
        throw GMWND_LAST_EXCEPT();
}

bool Graphics_DX::shouldClose() const
{
    return ecode != INT_MAX;
}

int Graphics_DX::GetExitCode() const noexcept
{
    return ecode;
}

void Graphics_DX::UpdateBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
    HRESULT hr;
    D3D11_MAPPED_SUBRESOURCE resource;
    GFX_THROW_INFO(pContext->Map(bufferToUpdate, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource));
    memcpy((char*)resource.pData + offset, data, dataSize);
    pContext->Unmap(bufferToUpdate, 0);
}

void Graphics_DX::SetRasterizerState() const
{
    pContext->RSSetState(rasterStates[(int)frontFace][(int)culling].Get());
}


Graphics_DX::VertexBuffer::VertexBuffer(ID3D11Device& device, const D3D11_BUFFER_DESC& description, const void* data, const D3D11_INPUT_ELEMENT_DESC* inputDesc, const FileContainer& shader)
    : Buffer_DX(device, description, data)
    , vertexSize(description.StructureByteStride)
{
    HRESULT hr;
    GFX_THROW_INFO(device.CreateInputLayout(inputDesc, 1, shader.Get(), shader.Size(), &layout));
}

void Graphics_DX::VertexBuffer::bind(ID3D11DeviceContext& context) const
{
    static const UINT offset = 0;
    context.IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &vertexSize, &offset);
    context.IASetInputLayout(layout.Get());
}

void Graphics_DX::VertexBuffer::Delete() const
{
    Buffer_DX::Delete();
    layout->Release();
}

template<>
void Graphics_DX::Shader_DX<ID3D11VertexShader>::Set(ID3D11Device& device)
{
    HRESULT hr;
    GFX_THROW_INFO(device.CreateVertexShader(file.Get(), file.Size(), nullptr, &shader));
}

template<>
void Graphics_DX::Shader_DX<ID3D11GeometryShader>::Set(ID3D11Device& device)
{
    HRESULT hr;
    GFX_THROW_INFO(device.CreateGeometryShader(file.Get(), file.Size(), nullptr, &shader));
}

template<>
void Graphics_DX::Shader_DX<ID3D11PixelShader>::Set(ID3D11Device& device)
{
    HRESULT hr;
    GFX_THROW_INFO(device.CreatePixelShader(file.Get(), file.Size(), nullptr, &shader));
}


// Window Exception Stuff
std::string Graphics_DX::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    // windows will allocate memory for err string and make our pointer point to it
    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
    );
    // 0 string length returned indicates a failure
    if (nMsgLen == 0)
    {
        return "Unidentified error code";
    }
    // copy error string from windows-allocated buffer to std::string
    std::string errorString = pMsgBuf;
    // free windows buffer
    LocalFree(pMsgBuf);
    return errorString;
}


Graphics_DX::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file),
    hr(hr)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* Graphics_DX::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;
    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics_DX::HrException::GetType() const noexcept
{
    return "GMan Graphics Exception";
}

HRESULT Graphics_DX::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Graphics_DX::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(hr);
}

std::string Graphics_DX::HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf));
    return buf;
}

std::string Graphics_DX::HrException::GetErrorInfo() const noexcept
{
    return info;
}


const char* Graphics_DX::DeviceRemovedException::GetType() const noexcept
{
    return "GMan Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics_DX::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}


const char* Graphics_DX::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics_DX::InfoException::GetType() const noexcept
{
    return "GMan Graphics Info Exception";
}

std::string Graphics_DX::InfoException::GetErrorInfo() const noexcept
{
    return info;
}
