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
	app.Get("/getMedias/:accId", media.GetMedias)
	app.Get("/health", media.GetAppHealth)
	app.Get("/insertMedia/:accId/:name", media.InsertMedia)
	app.Get("/nameExists/:name", media.NameExists)
	app.Get("/deleteMedia/:name", media.DeleteMedia)
	app.Get("/deleteMedias/:accId", media.DeleteMedias)
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
