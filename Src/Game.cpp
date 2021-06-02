#include "Game.h"
#include "Box2DHelper.h"

Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		CheckCollitions();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close();
			break;
		}
	}
}

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// MyContactListener* l= new MyContactListener();
	// phyWorld->SetContactListener(l);
	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// Creamos un circulo que ataremos al centro del cuerpo
	b2Body* cabeza = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5,7,0.4f,0.9,0.9);
	cabeza->SetTransform(b2Vec2(50.0f, 5.0f), 0.0f);

	// Creamos un circulo que ataremos al resorte derecho
	b2Body* cuerpo = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 10, 15, 0.7f, 0.1, 0.1);
	cuerpo->SetTransform(b2Vec2(50.0f, 20.0f), 0.0f);

	// Creamos un brazo derecho que ataremos al centro del cuerpo
	b2Body* brazo1 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 10, 0.3f, 0.2, 0.5);
	brazo1->SetTransform(b2Vec2(42.0f, 18.0f), -3.0f);

	// Creamos un brazo izquierdo que ataremos al centro del cuerpo
	b2Body* brazo2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 10, 0.3f, 0.2, 0.5);
	brazo2->SetTransform(b2Vec2(58.0f, 18.0f), 3.0f);

	// Creamos un pierna izquierda que ataremos al centro del cuerpo
	b2Body* pierna1 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 10, 0.4f, 0.0, 0.0);
	pierna1->SetTransform(b2Vec2(46.0f, 35.0f), 0.0f);

	// Creamos un pierna derecha que ataremos al centro del cuerpo
	b2Body* pierna2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 10, 0.4f, 0.0, 0.0);
	pierna2->SetTransform(b2Vec2(54.0f, 35.0f), 0.0f);

	//Aplico Resortes para unir el cuerpo con cabeza
	Box2DHelper::CreateDistanceJoint(phyWorld, cabeza,b2Vec2(cabeza->GetPosition().x, cabeza->GetPosition().y + 6.f),
		cuerpo, b2Vec2(cuerpo->GetPosition().x,cuerpo->GetPosition().y-7.f), 0.5f, 10.f, 0.5f);

	//Aplico Resortes para unir el brazo1 con cuerpo
		Box2DHelper::CreateDistanceJoint(phyWorld, brazo1, b2Vec2(brazo1->GetPosition().x + 0.8f, brazo1->GetPosition().y-5.f),
			cuerpo, b2Vec2(cuerpo->GetPosition().x - 1.f, cuerpo->GetPosition().y -7.f), 0.5f, 10.f, 0.5f);

	//Aplico Resortes para unir el brazo2 con cuerpo
		Box2DHelper::CreateDistanceJoint(phyWorld, brazo2, b2Vec2(brazo2->GetPosition().x - 0.8f, brazo2->GetPosition().y -5.f),
			cuerpo, b2Vec2(cuerpo->GetPosition().x + 2.f, cuerpo->GetPosition().y - 7.f), 0.5f, 10.f, 0.5f);

	//Aplico Resortes para unir el pierna1 con cuerpo
		Box2DHelper::CreateDistanceJoint(phyWorld, pierna1, b2Vec2(pierna1->GetPosition().x + 0.8f, pierna1->GetPosition().y - 5.f),
			cuerpo, b2Vec2(cuerpo->GetPosition().x - 2.f, cuerpo->GetPosition().y + 9.f), 0.5f, 10.f, 0.5f);

	//Aplico Resortes para unir el pierna1 con cuerpo
		Box2DHelper::CreateDistanceJoint(phyWorld, pierna2, b2Vec2(pierna2->GetPosition().x - 0.8f, pierna2->GetPosition().y - 5.f),
			cuerpo, b2Vec2(cuerpo->GetPosition().x + 2.f, cuerpo->GetPosition().y + 9.f), 0.5f, 10.f, 0.5f);

	

}

Game::~Game(void)
{
}