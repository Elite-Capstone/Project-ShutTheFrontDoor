package main

import "github.com/gofiber/fiber"
import "./media"

func main() {

	app := fiber.New()

	setUpAppRoutes(app)

	err := app.Listen(":6969")

	if err != nil {
		panic(err.Error())
	}
}

func setUpAppRoutes(app *fiber.App) {
	app.Get("/getMedias", media.GetMedias)
	app.Get("/health", media.GetAppHealth)
	app.Put("/insertMedia", media.GetMedias)
	app.Delete("/media", media.DeleteMedia)
	app.Delete("/medias", media.DeleteMedias)
}
