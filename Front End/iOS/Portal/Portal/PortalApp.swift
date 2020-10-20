//
//  PortalApp.swift
//  Portal
//
//  Created by Dean Chong San on 2020-10-19.
//

import SwiftUI

@main
struct PortalApp: App {
    let persistenceController = PersistenceController.shared

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environment(\.managedObjectContext, persistenceController.container.viewContext)
        }
    }
}
