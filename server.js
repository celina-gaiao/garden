/*
 * Copyright (c) 2016 Celina Gaião
 * https://github.com/celina-gaiao/garden
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

const debug = require('debug')('garden');
const five = require('johnny-five');
const chipio = require('chip-io');
const express = require('express');
const app = express();

app.disable('x-powered-by');

let solenoids = [];

let board = new five.Board({
    io: new chipio(),
    repl: false,
});
board.on('ready', function() {

    for (let i = 0; i < 4; i++) {
        solenoids[i] = new five.Relay({
            pin: `CSID${i*2}`, // use CSID0, CSID2, CSID4, CSID6
            type: 'NO', // normally open
        });
        solenoids[i].off();
    }

});

app.get('/solenoids/:color/:status', function(req, res, next) {

    // TODO do a better mapping
    let colors = ['brown', 'blue', 'orange', 'green'];
    let s = solenoids[colors.indexOf(req.params.color)];

    debug(`turning ${s.pin} ${req.params.status}`);

    req.params.status === 'on' ? s.on() : s.off();
    res.status(200).send();

    next();
});

app.use(function (err, req, res, next) {
    console.error(err.stack);
    res.status(500).send({code: 'InternalError'});
});

const port = process.env.PORT || 3000;
app.listen(port, () => {
    console.log(`Listening at http://localhost:${port}/`);
});
