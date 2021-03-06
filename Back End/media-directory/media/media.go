package media

import "github.com/gofiber/fiber"

func GetMedias(f *fiber.Ctx) error {
	return f.SendString("Getting the medias")
}

func InsertMedia(f *fiber.Ctx) error {
	return f.SendString("Insert")
}

func DeleteMedia(f *fiber.Ctx) error {
	return nil
}

func DeleteMedias(f *fiber.Ctx) error {
	return nil
}

func GetAppHealth(f *fiber.Ctx) error {
	return f.SendString("App is up and running!")
}