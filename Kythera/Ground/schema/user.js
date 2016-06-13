"use strict";
/*
 *  Defined the Mongoose Schema and return a Model for a User
 */
/* jshint node: true */

var mongoose = require('mongoose');

/*
 * User can have list of favorite photos
 */
var favoriteSchema = new mongoose.Schema({
    date_time: {type: Date, default: Date.now}, // The date and time when the comment was created.
    file_name: String    // 	The user object of the user who created the comment.
});

// create a schema
var userSchema = new mongoose.Schema({
    id: String,     // Unique ID identifying this user
    login_name: String, // Unique Login name for this user
    password: String,
    first_name: String, // First name of the user.
    last_name: String,  // Last name of the user.
    location: String,    // Location  of the user.
    description: String,  // A brief user description
    occupation: String,    // Occupation of the user.
    activity: String,
    favorites: [favoriteSchema]
});

// the schema is useless so far
// we need to create a model using it
var User = mongoose.model('User', userSchema, 'User');

// make this available to our users in our Node applications
module.exports = User;
