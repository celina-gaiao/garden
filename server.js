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
