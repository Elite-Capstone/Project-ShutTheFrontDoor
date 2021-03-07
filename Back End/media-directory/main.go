package main

import (
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"github.com/gofiber/fiber"
	"github.com/jinzhu/gorm"
	"os"
)
import "github.com/superDeano/media-directory/media"
import "github.com/superDeano/media-directory/dao"

func main() {

	app := fiber.New()

	setUpAppRoutes(app)
	setUpDatabase()
	defer closeDbConnection()

	err := app.Listen(os.Getenv("SERVER_PORT"))

	if err != nil {
		panic(err.Error())
	}
}

func setUpAppRoutes(app *fiber.App) {
	app.Get("mediaDirectory/getMedias/:accId", media.GetMedias)
	app.Get("mediaDirectory/health", media.GetAppHealth)
	app.Get("mediaDirectory/insertMedia/:accId/:name", media.InsertMedia)
	app.Get("mediaDirectory/nameExists/:name", media.NameExists)
	app.Get("mediaDirectory/deleteMedia/:name", media.DeleteMedia)
	app.Get("mediaDirectory/deleteMedias/:accId", media.DeleteMedias)
}

func setUpDatabase() {
	var err error
	var connectionString = os.Getenv("MYSQLDB")
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
