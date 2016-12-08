/*
 * Copyright (c) 2016 Celina Gaião
 * https://github.com/celina-gaiao/garden
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

const debug = require('debug')('garden');
const express = require('express');
const app = express();

app.disable('x-powered-by');

const actuators = require('./actuators');

actuators.init().then((solenoids) => {

    const schedule = require('./schedule');
    solenoids.map((s) => {
        switch (s.id) {
            case 'green':
                s.startTimeH = 7;
                s.startTimeM = 13;
                s.duration = 1;
                break;
            case 'blue':
                s.startTimeH = 7;
                s.startTimeM = 0;
                s.duration = 5;
                break;
            case 'orange':
                s.startTimeH = 7;
                s.startTimeM = 2;
                s.duration = 10;
                break;
        }
    });

    schedule.setSchedule(solenoids.filter(solenoid => solenoid.duration));

    app.get('/solenoids/:color/:status', function (req, res, next) {

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

});
