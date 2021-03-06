package main

import (
	"database/sql"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"github.com/gofiber/fiber"
)
import "./media"
import "./dao"

func main() {

	app := fiber.New()

	setUpAppRoutes(app)
	setUpDatabase()
	defer closeDbConnection()

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

func setUpDatabase() {
	var err error
	dao.Db, err = sql.Open("mysql", "")

	if err != nil {
		panic(err.Error())
	}
	fmt.Println("Successfully connected to Database")
}

func closeDbConnection() {
	err := dao.Db.Close()

	if err != nil {
		panic("Could not close db connection properly")
	}

	fmt.Println("Successfully close db connection")
}
