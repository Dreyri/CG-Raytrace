#include "simple.hpp"

void SimpleScene1::setup()
{
    this->tracer = rt::RaytracerSimple();
    this->tracer.numThreads = 4;
    this->scene = std::make_shared<rt::Scene>(rt::Scene());
    tracer.scene = this->scene;

    rt::Camera cam = rt::Camera();
    cam.position = rt::vec3(7.0, 2.0, 0.0);
    cam.direction = glm::normalize(rt::vec3(0.0, 0.0, 0.0) - cam.position); // Look at {0.0, 0.0, 0.0}
    cam.fov = 90;

    rt::AmbientLight ambient = rt::AmbientLight();
    ambient.color = rt::fColor(1.0f, 1.0f, 1.0f);
    ambient.intensity = 1.0;

    rt::Light light = rt::Light();
    light.position = rt::vec3(5.0, 8.0, -5.0);
    light.color = rt::fColor(1.0f, 1.0f, 1.0f);
    light.intensity = 1.0;

    scene->background = rt::fColor(0.5294117647, 0.80784313725, 0.92156862745);
    scene->camera = cam;
    scene->ambient = ambient;
    scene->light = light;

    std::shared_ptr<rt::SimpleMaterial> bronze = std::make_shared<rt::SimpleMaterial>();
    bronze->ambient = { 0.329412, 0.223529, 0.027451 };
    bronze->diffuse = { 0.780392, 0.568627, 0.113725 };
    bronze->specular = { 0.992157, 0.941176, 0.807843 };
    bronze->shininess = 27.8974;
    bronze->reflection_amount = 0.4;
    bronze->transparent = false;

    rt::Object box1 = rt::Object(rt::Mesh::getUnityCube(), bronze);
    box1.setPosition({ 0.0, 0.0, 4.0 });
    box1.setScale({ 2.0, 2.0, 2.0 });
    box1.setRotation(45.0, { 0.0, 1.0, 0.0 });
    scene->objects.push_back(box1);

    rt::Object sphere1 = rt::Object(rt::Mesh::getUnitySphere(), bronze);
    sphere1.setPosition({ 0.0, 0.0, -4.0 });
    sphere1.setScale({ 2.0, 2.0, 2.0 });
    sphere1.setRotation(-20.0, { 0.0, 1.0, 0.0 });
    scene->objects.push_back(sphere1);

    this->timer.start();
    this->frameCount = 0;
}

void SimpleScene1::transform(unsigned int depth, float adaptive, bool smoothing, bool reflection, bool refraction, bool animation, int slider, double index, int camHorizontal, int camVertical)
{
    this->frameCount++;

    tracer.maxDepth = depth;
    tracer.adaptiveDepth = adaptive;
    tracer.smoothing = smoothing;
    tracer.reflection = reflection;
    tracer.refraction = refraction;

    if (animation)
    {
        double boxRot = (ulong)(0.01 * this->timer.elapsed()) % 360;
        tracer.scene->objects[0].setRotation(glm::radians(boxRot), {0.0, 1.0, 0.0});
    }
    else
    {
        tracer.scene->objects[0].setRotation(glm::radians((double)slider), { 0.0, 1.0, 0.0 });
    }
    
    tracer.scene->transform();
}

void SimpleScene1::render(QImage& target)
{
    std::shared_ptr<ImageTarget> t = std::make_shared<ImageTarget>(target);
    tracer.render(t);
}

void SimpleScene2::setup()
{
    this->tracer = rt::RaytracerSimple();
    this->tracer.numThreads = 4;
    this->scene = std::make_shared<rt::Scene>(rt::Scene());
    tracer.scene = this->scene;

    rt::Camera cam = rt::Camera();
    cam.position = rt::vec3(9.0, 4, 0.0);
    cam.direction = glm::normalize(rt::vec3(0.0, 0.0, 0.0) - cam.position); // Look at {0.0, 0.0, 0.0}
    cam.fov = 90;

    rt::AmbientLight ambient = rt::AmbientLight();
    ambient.color = rt::fColor(1.0f, 1.0f, 1.0f);
    ambient.intensity = 1.0;

    rt::Light light = rt::Light();
    light.position = rt::vec3(5.0, 8.0, -5.0);
    light.color = rt::fColor(1.0f, 1.0f, 1.0f);
    light.intensity = 1.0;

    scene->background = rt::fColor(0.5294117647, 0.80784313725, 0.92156862745);
    scene->camera = cam;
    scene->ambient = ambient;
    scene->light = light;

    std::shared_ptr<rt::SimpleMaterial> bronze = std::make_shared<rt::SimpleMaterial>();
    bronze->ambient = { 0.329412, 0.223529, 0.027451 };
    bronze->diffuse = { 0.780392, 0.568627, 0.113725 };
    bronze->specular = { 0.992157, 0.941176, 0.807843 };
    bronze->shininess = 27.8974;
    bronze->reflection_amount = 0.4;
    bronze->transparent = false;

    std::shared_ptr<rt::SimpleMaterial> glass = std::make_shared<rt::SimpleMaterial>();
    glass->ambient = { 0.1, 0.1, 0.1 };
    glass->diffuse = { 0.1, 0.1, 0.1 };
    glass->specular = { 0.2, 0.2, 0.2 };
    glass->shininess = 40.0;
    glass->reflection_amount = 0.3;
    glass->transparent = true;
    glass->refraction_amount = 0.7;
    glass->refraction_index = 1.5;

    rt::Texture5x5* tex5x5 = new rt::Texture5x5();
    std::shared_ptr<rt::TextureMaterial> checkerdTex = std::make_shared<rt::TextureMaterial>(tex5x5);
    checkerdTex->ambient = 0.5;
    checkerdTex->diffuse = 1.0;
    checkerdTex->specular = 1.0;
    checkerdTex->shininess = 1.0;
    checkerdTex->reflection_amount = 0.0;
    checkerdTex->transparent = false;

    ImageTexture* mT = new ImageTexture(QImage("jade1.jpg"));
    std::shared_ptr<rt::TextureMaterial> metalTex = std::make_shared<rt::TextureMaterial>(mT);
    metalTex->ambient = 0.2;
    metalTex->diffuse = 0.3;
    metalTex->specular = 0.6;
    metalTex->shininess = 15.0;
    metalTex->reflection_amount = 0.4;
    metalTex->transparent = false;

    rt::Object box1 = rt::Object(rt::Mesh::getUnityCube(), bronze);
    box1.setPosition({ 0.0, -2.0, 0.0 });
    box1.setScale({ 20.0, 1.0, 20.0 });
    //box1.setRotation(45.0, { 0.0, 1.0, 0.0 });
    //scene->objects.push_back(box1);

    rt::Object sphere1 = rt::Object(rt::Mesh::getUnitySphere(), bronze);
    sphere1.setPosition({ 0.0, 2.0, -4.0 });
    sphere1.setScale({ 2.0, 2.0, 2.0 });
    scene->objects.push_back(sphere1);

    rt::Object planeXZ = rt::Object(rt::Mesh::getXZPlane(), checkerdTex);
    planeXZ.setPosition({ 0.0, -2.0, 0.0 });
    planeXZ.setScale({ 10.0, 0.0, 10.0 });
    scene->objects.push_back(planeXZ);

    rt::Object glass1 = rt::Object(rt::Mesh::getUnitySphere(), glass);
    glass1.setPosition({ 5.0, 2.0, 3.0 });
    glass1.setScale({ 2.0, 2.0, 2.0 });
    //glass1.setRotation(0.0, { 0.0, 1.0, 0.0 });
    scene->objects.push_back(glass1);

    rt::Object teapot = rt::Object(rt::Mesh::getObj("teapot2.obj"), checkerdTex);
    teapot.setPosition({ 5.5, 0.5, -4.0 });
    teapot.setScale({ 0.12, 0.12, 0.12 });
    teapot.setRotation(glm::radians(225.0), {0.0, 1.0, 0.0});
    scene->objects.push_back(teapot);
    
    this->timer.start();
    this->frameCount = 0;
}

void SimpleScene2::transform(unsigned int depth, float adaptive, bool smoothing, bool reflection, bool refraction, bool animation, int slider, double index, int camHorizontal, int camVertical)
{
    this->frameCount++;

    tracer.maxDepth = depth;
    tracer.adaptiveDepth = adaptive;
    tracer.smoothing = smoothing;
    tracer.reflection = reflection;
    tracer.refraction = refraction;

    tracer.scene->camera.hOff = camHorizontal;
    tracer.scene->camera.vOff = camVertical;

    tracer.scene->objects[2].material->refraction_index = index;

    if (animation)
    {
        double boxRot = (ulong)(0.01 * this->timer.elapsed()) % 360;
        tracer.scene->objects[1].setRotation(glm::radians(boxRot), { 0.0, 1.0, 0.0 });
    }
    else
    {
        tracer.scene->objects[1].setRotation(glm::radians((double)slider), { 0.0, 1.0, 0.0 });
    }

    tracer.scene->transform();
}

void SimpleScene2::render(QImage& target)
{
    std::shared_ptr<ImageTarget> t = std::make_shared<ImageTarget>(target);
    tracer.render(t);
}
