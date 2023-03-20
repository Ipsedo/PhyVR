//
// Created by samuel on 16/03/2023.
//

#include <android/sensor.h>

#include <android_native_app_glue.h>
#include <dlfcn.h>
#include <cassert>
#include <random>
#include <glm/gtx/transform.hpp>

#include "./utils/logging.h"
#include "./model/engine.h"
#include "./model/items.h"
#include "./view/renderer.h"
#include "./controller/controller.h"
#include "./view/drawable/specular.h"
#include "./view/drawable/cubemap.h"


// https://github.com/JustJokerX/NativeActivityFromJavaActivity/blob/master/app/src/main/cpp/main.cpp


struct saved_state {
    std::shared_ptr<Camera> camera;
    std::shared_ptr<ControllerEngine> controller_engine;
    std::shared_ptr<Engine> physic_engine;
    std::vector<std::shared_ptr<Item>> items;
    std::shared_ptr<std::mt19937> rng;
};

/**
 * Shared state for our app.
 */
struct engine {
    struct android_app *app;

    ASensorManager *sensorManager;
    const ASensor *accelerometerSensor;
    ASensorEventQueue *sensorEventQueue;

    std::shared_ptr<Renderer> renderer;

    saved_state *state;
};


static int32_t engine_handle_input(struct android_app *app, AInputEvent *event) {
    //struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        /*engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);*/
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app *app, int32_t cmd) {
    auto *engine = (struct engine *) app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(saved_state));
            engine->app->savedState = engine->state;
            engine->app->savedStateSize = sizeof(saved_state);
            LOG_INFO("save state");
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != nullptr) {
                if (engine->renderer && !engine->renderer->is_closed())
                    break;
                LOG_INFO("opening window");

                engine->renderer = std::make_shared<Renderer>(
                        engine->app->window,
                        engine->state->camera
                );
                engine->renderer->enable();
                engine->renderer->add_drawable("cubemap", std::make_shared<CubeMap>(
                        engine->app->activity->assetManager,
                        "cubemap/1"
                ));

                // TODO call first draw

                for (const auto &item: engine->state->items) {
                    std::uniform_real_distribution<float> u_dist(0., 1.);
                    glm::vec4 a_color(
                            u_dist(*engine->state->rng),
                            u_dist(*engine->state->rng),
                            u_dist(*engine->state->rng),
                            1.f
                    );
                    glm::vec4 d_color(
                            u_dist(*engine->state->rng),
                            u_dist(*engine->state->rng),
                            u_dist(*engine->state->rng),
                            1.f
                    );
                    glm::vec4 s_color(
                            u_dist(*engine->state->rng),
                            u_dist(*engine->state->rng),
                            u_dist(*engine->state->rng),
                            1.f
                    );
                    auto drawable = std::make_shared<Specular>(
                            engine->app->activity->assetManager,
                            item->get_shape()->get_vertices(),
                            item->get_shape()->get_normals(),
                            a_color,
                            d_color,
                            s_color,
                            50.f
                    );
                    engine->renderer->add_drawable(item->get_name(), drawable);
                }
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            //engine_term_display(engine);
            engine->renderer->close();
            LOG_INFO("close");
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            /*if (engine->accelerometerSensor != nullptr) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                                               engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                                               engine->accelerometerSensor,
                                               (1000L / 60) * 1000);
            }*/
            engine->renderer->enable();
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            /*if (engine->accelerometerSensor != nullptr) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                                                engine->accelerometerSensor);
            }*/
            // Also stop animating.
            engine->renderer->disable();
            /*engine_draw_frame(engine);*/
            break;
        default:
            break;
    }
}

/*
 * AcquireASensorManagerInstance(void)
 *    Workaround ASensorManager_getInstance() deprecation false alarm
 *    for Android-N and before, when compiling with NDK-r15
 */

ASensorManager *AcquireASensorManagerInstance(android_app *app) {

    if (!app)
        return nullptr;

    typedef ASensorManager *(*PF_GETINSTANCEFORPACKAGE)(const char *name);
    void *androidHandle = dlopen("libandroid.so", RTLD_NOW);
    auto getInstanceForPackageFunc = (PF_GETINSTANCEFORPACKAGE)
            dlsym(androidHandle, "ASensorManager_getInstanceForPackage");
    if (getInstanceForPackageFunc) {
        JNIEnv *env = nullptr;
        app->activity->vm->AttachCurrentThread(&env, nullptr);

        jclass android_content_Context = env->GetObjectClass(app->activity->clazz);
        jmethodID midGetPackageName = env->GetMethodID(android_content_Context,
                                                       "getPackageName",
                                                       "()Ljava/lang/String;");
        auto packageName = (jstring) env->CallObjectMethod(app->activity->clazz,
                                                           midGetPackageName);

        const char *nativePackageName = env->GetStringUTFChars(packageName, nullptr);
        ASensorManager *mgr = getInstanceForPackageFunc(nativePackageName);
        env->ReleaseStringUTFChars(packageName, nativePackageName);
        app->activity->vm->DetachCurrentThread();
        if (mgr) {
            dlclose(androidHandle);
            return mgr;
        }
    }

    typedef ASensorManager *(*PF_GETINSTANCE)();
    auto getInstanceFunc = (PF_GETINSTANCE)
            dlsym(androidHandle, "ASensorManager_getInstance");
    // by all means at this point, ASensorManager_getInstance should be available
    assert(getInstanceFunc);
    dlclose(androidHandle);

    return getInstanceFunc();
}


/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app *state) {
    engine engine{};

    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = AcquireASensorManagerInstance(state);
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(
            engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(
            engine.sensorManager,
            state->looper, LOOPER_ID_USER,
            nullptr, nullptr);

    if (state->savedState != nullptr) {
        // We are starting with a previous saved state; restore from it.
        engine.state = (saved_state *) state->savedState;
        LOG_INFO("load state");
    } else {
        engine.state = new saved_state{};

        std::random_device dev;
        engine.state->rng = std::make_shared<std::mt19937>(dev());

        engine.state->camera = std::make_shared<StaticCamera>(
                glm::vec3(0., 10., 0.),
                glm::vec3(0., 10., 1.),
                glm::vec3(0., 1., 0.)
        );
        engine.state->controller_engine = std::make_shared<ControllerEngine>();
        engine.state->physic_engine = std::make_shared<Engine>();

        auto map = std::make_shared<HeightMapItem>(
                "height_map",
                state->activity->assetManager,
                "heightmap/heightmap6_2.png",
                glm::vec3(0., 0., 0.),
                glm::vec3(4., 40., 4.)
        );

        engine.state->items.push_back(map);
        engine.state->physic_engine->add_item(map);
    }

    // loop waiting for stuff to do.

    while (true) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source *source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident = ALooper_pollAll(
                engine.renderer != nullptr && engine.renderer->is_enabled() ? 0 : -1, nullptr,
                &events,
                (void **) &source)) >= 0) {

            // Process this event.
            if (source != nullptr) {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != nullptr) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                                                       &event, 1) > 0) {
                        /*LOG_INFO("accelerometer: x=%f y=%f z=%f",
                             event.acceleration.x, event.acceleration.y,
                             event.acceleration.z);*/
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine.renderer->close();
                LOG_INFO("closing PhyVR");
                return;
            }
        }

        if (engine.renderer->is_enabled()) {
            engine.state->physic_engine->step(1. / 60.);

            std::vector<std::tuple<std::string, glm::mat4>> res{};
            std::transform(
                    engine.state->items.begin(), engine.state->items.end(),
                    std::back_inserter(res),
                    [](const std::shared_ptr<Item> &item) {
                        return std::tuple<std::string, glm::mat4>(item->get_name(),
                                                                  item->get_model_matrix());
                    });

            res.emplace_back("cubemap", glm::scale(glm::mat4(1.), glm::vec3(2000., 2000., 2000.)));

            engine.renderer->draw(res);
        }
    }
}