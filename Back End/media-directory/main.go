package main

import (
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"github.com/gofiber/fiber"
	"github.com/jinzhu/gorm"
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
	app.Get("/getMedias/:id", media.GetMedias)
	app.Get("/health", media.GetAppHealth)
	app.Put("/insertMedia", media.GetMedias)
	app.Delete("/media/:name", media.DeleteMedia)
	app.Delete("/medias", media.DeleteMedias)
}

func setUpDatabase() {
	var err error
	var connectionString = ""
	dao.Db, err = gorm.Open("mysql", connectionString)
	if err != nil {
		panic(err.Error())
	}
	fmt.Println("Successfully connected to Database")

	dao.Db.AutoMigrate(&media.Media{})
}

func closeDbConnection() {
	err := dao.Db.Close()

	if err != nil {
		panic("Could not close db connection properly")
	}

	fmt.Println("Successfully close db connection")
}
